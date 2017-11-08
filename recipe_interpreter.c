/*
 * File : recipe_interpreter.c
 * Description : Routines for configuring and processing servo recipes
 *               in a Process Control Block
 * Author : Chris Ranc
 */

#include "recipe_interpreter.h"
#include "PWM.h"

#include <stdint.h>

/* Processes the current state of the Recipe Process Control Block
 * and evaluate the next instruction or the entered user input to
 * determine the state
 */
void process(recipe_process* proc){
    if (proc->u_state == user_input){
        proc->u_state = no_input;
        eval_user(proc);
    }else {
        switch (proc->p_state){
            case processing:
                eval_instr(proc);
                break;
            case servo_running:
            case waiting:
                if (proc->wait_cnt == 0){
                    proc->p_state = processing;
                    eval_instr(proc);
                }
            default:
                break;
        }
    }

}


/* Initialize Recipe Process Control Block with recipe and default starting
 * states of which is to begin as paused
 */
void init_process(recipe_process* proc, unsigned char* recipe, enum pwm_ch ch){
    proc->head_instr = recipe;
    proc->current_instr = recipe;

    proc->loop_cnt = 0;
    proc->wait_cnt = 0;

    proc->d_state = is_paused;
    proc->s_state = paused;
    proc->p_state = processing;
    proc->u_state = no_input;
    proc->l_state = not_looping;

    proc->servo_position = 0;
    proc->servo_channel = ch;

    mov(proc, 0);
}

/* Reset Recipe Process Block to begin at first instruction
 * and to be in the running state immediatley
 */
void reset_process(recipe_process* proc){
    proc->current_instr = proc->head_instr;

    proc->loop_cnt = 0;
    proc->wait_cnt = 0;

    proc->d_state = is_running;
    proc->s_state = not_paused;
    proc->p_state = processing;
    proc->u_state = no_input;
    proc->l_state = not_looping;
}

/* Evaluate and Apply Operation of Recipe Instruction */
void eval_instr(recipe_process* proc){
    unsigned char instr = *(proc->current_instr);
    uint8_t opcode = getInstrOpCode(instr);
    uint8_t instr_val = getInstrVal(instr);

    switch (opcode){
        case MOV:
            mov(proc, instr_val);
            mov_wait(proc);
            break;
        case WAIT:
            wait(proc, instr_val);
            break;
        case LOOP:
            loop(proc, instr_val);
            break;
        case END_LOOP:
            end_loop(proc);
            break;
        case RECIPE_END:
            break;
        default:
            proc->p_state = error;
            proc->d_state = command_error;
            break;
    }
    if (opcode != RECIPE_END ){
        proc->current_instr++;
    }
}

/* Move Servo Specified in Recipe Process Block */
void mov(recipe_process* proc, uint8_t position){
    uint32_t duty_cycle;

    if (MIN_SERVO_POS <= position && position <= MAX_SERVO_POS ){
        proc->servo_position = position;
        duty_cycle = (uint32_t) ((position * DUTY_CYCLE_INC) + BASE_DUTY_CYCLE);
        PWM_CH_Set(duty_cycle, proc->servo_channel);
    }else {
        proc->p_state = error;
        proc->d_state = command_error;
    }
}

/* Set Wait Count of Recipe Process Block */
void set_wait(recipe_process* proc, uint8_t wait_time){
    proc->wait_cnt = wait_time;
}

/* Set Wait Count and State Conditions of Recipe Process Block
 * for when the Servo is Moving 
 */
void mov_wait(recipe_process* proc){
    proc->p_state = servo_running;
    set_wait(proc, SERVO_WAIT_TIME);
}

/* Set Wait Count and State Conditions of Recipe Process Block
 * for When the Servo is to wait and not move
 */
void wait(recipe_process* proc, uint8_t wait_time){
    proc->p_state = waiting;
    set_wait(proc, wait_time);
}

/* Configure Recipe Process Control Block to Exhibit Loop
 * Behaviour by Storing the Start Loop Addresss and the
 * Amount of repetitions to occur 
 */
void loop(recipe_process* proc, uint8_t cnt){
    if (proc->l_state == looping){
        proc->p_state = error;
        proc->d_state = nested_loop_error;
        return;
    }
    proc->l_state = looping;
    proc->loop_instr = proc->current_instr;
    proc->loop_cnt = cnt;
}

/* Loop Back to Initial Loop Address and Decrement Loop Count
 * or Set Recipe Process Block State to No Longer Looping
 */
void end_loop(recipe_process* proc){
    if (proc->loop_cnt > 0){
        proc->current_instr = proc->loop_instr;
        proc->loop_cnt--;
        return;
    }
    proc->l_state = not_looping;
}

/* Evaluate user input and adjust Process State and 
 * behaviour based on User Input Commands
 * - p/P Pause recipe if not in an error or end state
 *
 * - c/C Continue recipe if paused and not in an error or end state
 *
 * - l/L Move servo left once if paused and not outside of possible
 *       positions
 *
 * - r/R Move servo right once if paused and not outside of possible
 *       positions
 *
 * - b/B Reset recipe process block to beginning of recipe and begin 
 *       running
 *
 * - n/N No operation and continue to process recipe process control block
 */
void eval_user(recipe_process* proc){
    uint8_t cmd = proc->user_instr;
    enum user_state state_u = proc->u_state;
    enum pause_state state_s = proc->s_state;
    proc->u_state = no_input;

    switch (cmd) {
    /* Pause */
    case 'P' :
    case 'p' :
        if (state_u != recipe_end && state_u != error &&
            state_s == not_paused){
            proc->s_state = paused;
            proc->d_state = is_paused;
        }
        break;

    /* Continue */
    case'C' :
    case'c' :
        if (state_u != recipe_end && state_u != error &&
            state_s == paused){
            proc->s_state = not_paused;
            proc->d_state = is_running;
        }
        break;

    /* Move Left */
    case 'L' :
    case 'l' :
        if (proc->servo_position < MAX_SERVO_POS && state_s == paused) {
            mov(proc, ++(proc->servo_position));
        }
        break;

    /* Move Right */
    case 'R' :
    case 'r' :
        if (proc->servo_position > MIN_SERVO_POS && state_s == paused)
            mov(proc, --(proc->servo_position));
        break;

    /* Restart */
    case 'B' :
    case 'b' :
        reset_process(proc);
        process(proc);
        break;
    /* No Operation */
    case 'N' :
    case 'n' :
        if (state_u != recipe_end && state_u != error &&
            state_s == not_paused)
            process(proc);
        break;
    /* Bad Command */
    default:
        break;
    }
}

