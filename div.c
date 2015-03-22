#include<stdio.h>
#include<math.h>
#include<gmp.h>

int main(){

char ch[100] ;
mpz_t a;
mpz_t b;
mpz_t c;
//mpz_init(a);
mpz_init_set_str(a, "123",10);//

//mpz_init(b);
mpz_init_set_str(b, "100" , 10);

mpz_init(c);
mpz_fdiv_q(c, a ,b);
mpz_get_str(ch, 10, c);
printf("%s\n",ch);

gmp_printf("%Z",c);
return 0;
}
