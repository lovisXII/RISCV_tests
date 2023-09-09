/*
    * convention for function name is : function_return-type_param1-type_param2-type
    * by default param1-type & param2-type are same than return
    * _int : return int
    * _uint : return unsigned int
    * _l : return long
    * _ul : return unsigned long
*/

int mul_int(int a,int b){
    int c = 0;
    for(int i= 0; i < b; i++){
        c += a;
    }
    return c;
}

unsigned int mul_uint(unsigned int a,unsigned int b){
    unsigned int c = 0;
    for(int i= 0; i < b; i++){
        c += a;
    }
    return c;
}


long mul_l(long a,long b){
    long c = 0;
    for(int i= 0; i < b; i++){
        c += a;
    }
    return c;
}

unsigned long mul_ul(unsigned long a,unsigned long b){
    unsigned long c = 0;
    for(int i= 0; i < b; i++){
        c += a;
    }
    return c;
}

unsigned long mul_ul_ul_uint(unsigned long a,unsigned int b){
    unsigned long c = 0;
    for(int i= 0; i < b; i++){
        c += a;
    }
    return c;
}

unsigned long power_ul(unsigned long a, unsigned long power){
    if(power == 0)
        return 1;    
    unsigned long b = a;
    for(int i = 1; i < power; i++){
        b = mul_ul_ul_uint(b,a);
    }
    return b;

}


int modulo(int a, int b){
    while(a >= b) a -= b;
    return a ;
}

