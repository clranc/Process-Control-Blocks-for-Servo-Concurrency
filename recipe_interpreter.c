#include "recipe_interpreter.h"
#include "PWM.h"

#include <stdint.h>

void master_proces(recipe_process* proc1, recipe_process* proc2 ){
    process(proc1);
    process(proc2);
}

void process(recipe_process* proc){
    switch(proc->cur_state){
        case running :
            eval_instr(proc);
            break;
        case user_cmd:
            eval_user(proc);
            break;
        case paused :
            break;
        case recipe_end :
            break;
    }
}

void eval_instr(recipe_process* proc){
    char instr = *(proc->current_instr); 
    uint8_t opcode = getInstrOpCode(instr);
    uint8_t instr_val = getInstrOpCode(instr);

    switch (opcode){
        case MOV:
            break;
        case WAIT:
            break;
        case LOOP:
            break;
        case END_LOOP:
            break;
        case RECIPE_END:
            break;
        default:
            break;
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

