

#include "PWM.h"

#include <stdint.h>

// Recipe OpCodes
#define MOV        0x20
#define WAIT       0x40
#define LOOP       0x80
#define END_LOOP   0xA0
#define RECIPE_END 0x00

// Instruction Parsers
#define getInstrOpCode(x) (uint8_t)(x & 0xE0)
#define getInstrVal(x) (uint8_t)(x & 0x1F)

// Servo Positions
#define MIN_SERVO_POS (uint8_t) 0
#define MAX_SERVO_POS (uint8_t) 5

// Servo PWM Values
#define BASE_DUTY_CYCLE 400
#define DUTY_CYCLE_INC 320

typedef struct recipe_process
{
    char* head_instr;
    char* current_instr;
    char* loop_instr;
    char user_instr;
    uint8_t loop_cnt;
    enum process_state p_state;
    enum user_state u_state;
    uint8_t servo_position;
    enum pwm_ch servo_channel;
}recipe_process;

enum process_state{
    running,
    paused,
    recipe_end
};

enum user_state{
    user_input,
    no_input
};

// Evaluation and Process Functions
void master_process(recipe_process* proc1, recipe_process* proc2);
void process(recipe_process* proc);
void eval_instr(recipe_process* proc);
void eval_user(recipe_process* proc);

// Opcode related functions
void mov(recipe_process * proc, uint8_t position);

