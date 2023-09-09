// #define STANDARD_LIB

#ifdef STANDARD_LIB
    #include <stdio.h>
#endif

#define LOOP_1_IT 20
#define LOOP_2_IT 10 
#define LOOP_3_IT 10 
#define LOOP_4_IT 5 


#ifndef STANDARD_LIB
extern void _bad();
extern void _good();

__asm__(".section .text") ;
__asm__(".global _start") ;


__asm__("_start:");
__asm__("addi x1,x1, 4");
__asm__("sub x2, x2,x1 ");
__asm__("jal x5, main");

#endif

#include "../func_lib/lib.h"


int main() {
    int a = 1;
    int nbr_adding = 13;
    unsigned long supposed_result = a;
    // We excute LOOP_1_IT*LOOP_2_IT*LOOP_3_IT*LOOP_4_IT times
    
    for(int i = 0; i < LOOP_1_IT; i++){
        for(int k = 0; k < LOOP_2_IT; k++){
            for(int l = 0; l < LOOP_3_IT; l++){
                for(int m = 0; m < LOOP_4_IT; m++){
                a += nbr_adding;
            }
        }
    }
   }
   
   unsigned long nbr_it = mul_ul_ul_uint(LOOP_1_IT,
   mul_ul_ul_uint(LOOP_2_IT,
   mul_ul_ul_uint(LOOP_3_IT,LOOP_4_IT)));

   supposed_result += mul_ul_ul_uint(nbr_adding,nbr_it) ;
  
   #ifdef STANDARD_LIB
   printf("result is : %ld\n", a);
   printf("suppose result is : %ld\n",supposed_result);
   #endif

   #ifndef STANDARD_LIB
   if(a == supposed_result)
        _good();
    else{
        _bad();
    }
    #endif
}