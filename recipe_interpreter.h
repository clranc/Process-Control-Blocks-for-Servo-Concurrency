// Recipe OpCodes
#define MOV        0x20
#define WAIT       0x40
#define LOOP       0x80
#define END_LOOP   0xA0
#define RECIPE_END 0x00

#define getInstrOpCode(x) (uint8_t)(x & 0xE0)
#define getInstrVal(x) (uint8_t)(x & 0x1F)

typedef struct recipe_process
{
    char* head_instr;
    char* current_instr;
    char* loop_instr;
    char user_instr;
    uint8_t loop_cnt;
    process_state cur_state;
}recipe_process;

enum process_state{
    running,
    user_cmd,
    paused,
    recipe_end
} process_state;

void master_process(recipe_process* proc1, recipe_process* proc2);
void process(recipe_process* proc);
void eval_instr(recipe_process* proc);
void eval_user(recipe_process* proc);
