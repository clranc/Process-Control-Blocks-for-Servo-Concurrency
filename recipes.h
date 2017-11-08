/* Recipes */
#define DEMO_RECIPE {MOV | 0 ,\
                     MOV | 5,\
                     MOV | 0,\
                     MOV | 3,\
                     LOOP | 3,\
                     MOV | 1,\
                     MOV | 4,\
                     END_LOOP,\
                     MOV | 0,\
                     MOV | 2,\
                     WAIT | 0,\
                     MOV | 3,\
                     WAIT | 0,\
                     MOV | 2,\
                     MOV | 3,\
                     WAIT | 31,\
                     WAIT | 31,\
                     WAIT | 31,\
                     MOV | 4,\
                     RECIPE_END\
                    }

#define POSITION_RECIPE {MOV | 0 ,\
                         MOV | 1,\
                         MOV | 2,\
                         MOV | 3,\
                         MOV | 4,\
                         MOV | 5,\
                         RECIPE_END\
                        }

#define REV_POSITION_RECIPE {MOV | 5,\
                             MOV | 4,\
                             MOV | 3,\
                             MOV | 2,\
                             MOV | 1,\
                             MOV | 0,\
                             RECIPE_END\
                            }

#define WAIT_RECIPE {MOV | 0,\
                     WAIT | 31,\
                     WAIT | 31,\
                     WAIT | 31,\
                     MOV | 5,\
                     RECIPE_END\
                    }

// Error Prone Recipes

// Nested Loop Error
#define LOOP_ERROR_RECIPE {MOV | 0 ,\
                           MOV | 1,\
                           MOV | 2,\
                           LOOP | 2,\
                           MOV  | 5,\
                           LOOP | 0,\
                           MOV | 1,\
                           END_LOOP,\
                           END_LOOP,\
                           MOV | 3,\
                           MOV | 4,\
                           MOV | 5,\
                           RECIPE_END\
                          }

// Command Error Recipes
#define BAD_RANGE_RECIPE {MOV | 0 ,\
                          MOV | 1,\
                          MOV | 2,\
                          MOV | 3,\
                          MOV | 4,\
                          MOV | 6,\
                          MOV | 5,\
                          RECIPE_END\
                         }

#define BAD_INSTR_RECIPE {MOV | 5 ,\
                          MOV | 4,\
                          MOV | 3,\
                          MOV | 2,\
                          MOV | 1,\
                          0xFF,\
                          MOV | 0,\
                          RECIPE_END\
                         }


