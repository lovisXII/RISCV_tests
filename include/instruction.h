#pragma once
#include "useful.h"
/*------------------------------------------------------------
                    INSTRUCTION CLASS
--------------------------------------------------------------*/

class Instruction{
    public:
                        Instruction(string name, int is_immediat, int type);
        inline int      getType()    {return _type ;                          };
        inline int      IsImmediat() {return (_is_immediat == 1) ? 1 : 0 ;    };
        inline string   getName()    {return _name ;                          };
        string          toString(int rd, int rs1, int rs2, string label);
    private:
        int     _is_immediat;
        int     _type;
        string  _name;
};