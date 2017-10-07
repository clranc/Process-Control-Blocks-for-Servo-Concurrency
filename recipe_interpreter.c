#include "recipe_interpreter.h"
#include "PWM.h"

#include <stdint.h>

void process(recipe_process* proc){

    if (proc->u_state == user_input){
        proc->u_state = no_input;
        eval_user(proc);
    }else {
        switch (proc->p_state){
            case running:
                eval_instr(proc);
                break;
            case waiting:
                if (proc->wait_cnt == 0){
                    proc->p_state = running;
                    eval_instr(proc);
                }
            default:
                break;
        }
    }
}

void initProcess(recipe_process* proc, char* recipe, enum pwm_ch ch){
    proc->head_instr = recipe;
    proc->current_instr = recipe;
    proc->loop_cnt = 0;
    proc->wait_cnt = 0;
    proc->p_state = running;
    proc->u_state = no_input;

    proc->servo_position = 0;
    proc->servo_channel = ch;
    
    mov(proc, 0);
}

void eval_instr(recipe_process* proc){
    char instr = *(proc->current_instr); 
    uint8_t opcode = getInstrOpCode(instr);
    uint8_t instr_val = getInstrVal(instr);

    switch (opcode){
        case MOV:
            mov(proc, instr_val);
            wait(proc, 2);
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
        default:
            break;
    }
    if (opcode != RECIPE_END ){
        proc->current_instr++;
    }
}

void mov(recipe_process* proc, uint8_t position){
    uint32_t duty_cycle;

    if (MIN_SERVO_POS <= position && position <= MAX_SERVO_POS ){
        proc->servo_position = position;
        duty_cycle = (uint32_t) (position * DUTY_CYCLE_INC + BASE_DUTY_CYCLE);
        PWM_CH_Set(duty_cycle, proc->servo_channel);
    }else {
        proc->p_state = error;
    }
}

void wait(recipe_process* proc, uint8_t wait_time){
    if (proc.loop_cnt > 0){
        
    }
    proc->wait_cnt = wait_time;
    proc->p_state = waiting;
}

void loop(recipe_process* proc, uint8_t cnt){
    proc->loop_instr = proc->current_instr;
    proc->loop_cnt = cnt;
}

void end_loop(recipe_process* proc){
    if (proc->loop_cnt > 0){
        proc->current_instr = proc->loop_instr;
        proc->loop_cnt--;
    }
}

void eval_user(recipe_process* proc){
    char cmd = proc->user_instr;
    
    // Pause
    if (cmd == 'p' || cmd == 'P'){

    // Continue
    }else if (cmd == 'c' || cmd == 'C'){

    // Move Right
    }else if (cmd == 'r' || cmd == 'R'){

    // Move Left
    }else if (cmd == 'l' || cmd == 'L'){

    // No Operation
    }else if (cmd == 'n' || cmd == 'N'){

    // Restart
    }else if (cmd == 'b' || cmd == 'B'){

    // Bad Command
    }else {

    }

}

