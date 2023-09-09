// #define STANDARD_LIB

#ifdef STANDARD_LIB
    #include <stdio.h>
#endif

#define LOOP_1_IT 2 
#define LOOP_2_IT 2 
#define LOOP_3_IT 2 
#define LOOP_4_IT 7 


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
    unsigned long b = 1;
    unsigned int nbr_multiplied = 2;

    // We excute LOOP_1_IT*LOOP_2_IT*LOOP_3_IT*LOOP_4_IT times
    
    for(int i = 0; i < LOOP_1_IT; i++){
        for(int k = 0; k < LOOP_2_IT; k++){
            for(int l = 0; l < LOOP_3_IT; l++){
                for(int m = 0; m < LOOP_4_IT; m++){
                b = mul_ul_ul_uint(b, nbr_multiplied);
            }
        }
    }
   }
   
   unsigned long power_nbr = mul_ul_ul_uint(LOOP_1_IT,
   mul_ul_ul_uint(LOOP_2_IT,
   mul_ul_ul_uint(LOOP_3_IT,LOOP_4_IT)));
   unsigned long supposed_result = power_ul((unsigned long)nbr_multiplied,power_nbr);
  
   #ifdef STANDARD_LIB
    printf("result is : %ld\n", b);
   printf("suppose result is : %ld\n",supposed_result);
   #endif

   #ifndef STANDARD_LIB
   if(b == supposed_result)
        _good();
    else{
        _bad();
    }
    #endif
}