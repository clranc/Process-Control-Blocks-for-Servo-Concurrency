
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

// Servo Values
//#endif /* RECIPE_INTERPRETER_H */
#define BASE_DUTY_CYCLE 400
#define DUTY_CYCLE_INC 320
#define SERVO_WAIT_TIME 8

enum process_state{
    processing,
    servo_running,
    waiting,
    error,
    recipe_end
};

enum pause_state{
    paused,
    not_paused
};

enum user_state{
    user_input,
    no_input
};

enum loop_state{
    looping,
    not_looping,
};

enum led_state{
    is_running,
    is_paused,
    nested_loop_error,
    command_error
};

typedef struct recipe_process
{
    unsigned char* head_instr;
    unsigned char* current_instr;
    unsigned char* loop_instr;
    uint8_t user_instr;

    uint8_t loop_cnt;
    uint8_t wait_cnt;

    enum led_state d_state;
    enum pause_state s_state;
    enum process_state p_state;
    enum user_state u_state;
    enum loop_state l_state;
    uint8_t servo_position;
    enum pwm_ch servo_channel;
}recipe_process;

// Recipe Process Initalizer
void init_process(recipe_process* proc, unsigned char*, enum pwm_ch ch);
void reset_process(recipe_process* proc);

// Evaluation and Process Functions
void process(recipe_process* proc);
void eval_instr(recipe_process* proc);
void eval_user(recipe_process* proc);

// Opcode related functions
void mov(recipe_process* proc, uint8_t position);
void set_wait(recipe_process* proc, uint8_t wait_time);
void mov_wait(recipe_process* proc);
void wait(recipe_process* proc, uint8_t wait_time);
void loop(recipe_process* proc, uint8_t cnt);
void end_loop(recipe_process* proc);
