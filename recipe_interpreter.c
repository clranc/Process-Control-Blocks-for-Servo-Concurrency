#include "recipe_interpreter.h"
#include "PWM.h"

#include <stdint.h>

void master_proces(recipe_process* proc1, recipe_process* proc2 ){
    process(proc1);
    process(proc2);
}

void process(recipe_process* proc){

    if (proc->u_state == user_input){
        proc->u_state = no_input;
        eval_user(proc);
    }else if (proc->p_state == running){
        eval_instr(proc);
    }
}

void eval_instr(recipe_process* proc){
    char instr = *(proc->current_instr); 
    uint8_t opcode = getInstrOpCode(instr);
    uint8_t instr_val = getInstrOpCode(instr);

    switch (opcode){
        case MOV:
            mov(proc, instr_val);
            break;
        case WAIT:
            break;
        case LOOP:
            break;
        case END_LOOP:
            break;
        default:
            break;
    }
    if (opcode != RECIPE_END){
        proc->current_instr++;
    }
}

void mov(recipe_process * proc, uint8_t position){
    uint32_t duty_cycle;

    if (MIN_SERVO_POS <= position && position <= MAX_SERVO_POS ){
        proc->servo_position = position;
        duty_cycle = (uint32_t) (position * DUTY_CYCLE_INC + BASE_DUTY_CYCLE);
        PWM_CH_Set(duty_cycle, proc->servo_channel);
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

