#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<math.h>
#include<string.h>

#define BASE 10
#define N 128

#define rep(i,n) for(i=0;i<n;i++)

#define POSITIVE 1
#define NEGATIVE 0

struct BigInteger{
	int digits[N];
	int sign;
	int length;
};

typedef struct BigInteger BigInteger;

BigInteger init(){
	int i;
	BigInteger c;
	rep(i, N)
		c.digits[i]=0;
	c.sign = 1;
	c.length = 1;
	return c;
}

void make_int (int A[], int n) {
	int	i;

	/* start indexing at the 0's place */

	i = 0;

	/* while there is still something left to the number
	 * we're encoding... */

	while (n) {

		/* put the least significant digit of n into A[i] */

		A[i++] = n % BASE;

		/* get rid of the least significant digit,
		 * i.e., shift right once
		 */

		n /= BASE;
	}

	/* fill the rest of the array up with zeros */

	while (i < N) A[i++] = 0;
}



void increment (int A[]) {
	int	i;

	/* start indexing at the least significant digit */

	i = 0;
	while (i < N) {

		/* increment the digit */

		A[i]++;

		/* if it overflows (i.e., it was 9, now it's 10, too
		 * big to be a digit) then...
		 */
	
		if (A[i] == BASE) {

			/* make it zero and index to the next 
			 * significant digit 
			 */
			A[i] = 0;
			i++;
		} else 
			/* otherwise, we are done */
			break;
	}
}

int check_max(BigInteger A, BigInteger B){

	int i;

	for(i=N-1; i>=0; i--) {
		if(A.digits[i]>B.digits[i]){
			return 1;
		}
		else if(A.digits[i]==B.digits[i])
			continue;
		else
			return 0;
	}

	return 2;
}







BigInteger subtract( BigInteger A, BigInteger B){ // const BigIntegerData left, const BigIntegerData right )

	int boo = check_max(A, B);
	if(!boo){
		printf("Invalid Subtraction\n");
	}	

  	BigInteger C = init();

  	int i = 0 , diff;

  	int borrow = 0;

  	for(i = 0; i < N; i++) {

  		diff  = A.digits[i] - B.digits[i] + borrow;

  		//printf("%d",diff);
  		if(diff < 0){
  			diff+=BASE;

  			borrow = 1;

  		}else{

  			borrow = 0;

  		}

  		C.digits[i] = diff;


  	}

  	rep(i, N)
		if(C.digits[N-1-i]!=0)
			break;
	C.length = N - i;
	
	if(N-i == 0)
		C.length = 1;

    return C;
}


BigInteger add (BigInteger A, BigInteger B) {

	int	i, carry, sum;

	BigInteger C = init();
	/* no carry yet */

	carry = 0;

	/* go from least to most significant digit */

	for (i=0; i<N; i++) {

		/* the i'th digit of C is the sum of the
		 * i'th digits of A and B, plus any carry
		 */
		sum = A.digits[i] + B.digits[i] + carry;

		/* if the sum exceeds the base, then we have a carry. */

		if (sum >= BASE) {

			carry = 1;

			/* make sum fit in a digit (same as sum %= BASE) */

			sum -= BASE;
		} else
			/* otherwise no carry */

			carry = 0;

		/* put the result in the sum */

		C.digits[i] = sum;
	}

 	rep(i, N)
		if(C.digits[N-1-i]!=0)
			break;

	C.length = N - i;
	//rep(i, N)

	return C;
	/* if we get to the end and still have a carry, we don't have
	 * anywhere to put it, so panic! 
	 */
	if (carry){ printf ("overflow in addition!\n"); return C;}
}



BigInteger *knuth_divide(BigInteger A, BigInteger B, BigInteger C[]){
	int i,j,m,n,num,k=0,l;
	BigInteger P = init();	
	C[0] = init();
	C[1] = init();
	//BigInteger C[1] = init();
	int boo = check_max(A, B);

	for(i=N-1;i>=0,A.digits[i]==0;i--);

	for(j=N-1;j>=0,B.digits[j]==0;j--);
		
	n = i;
	m = j;

	

	printf("m %d n  %d\n", m+1 , n+1 );
	n++;
	k=0;
	l=0;
	if(m==0){
		 for(i=n;i>=1;i--){	
			num = A.digits[i]*10 + A.digits[i-1];
			A.digits[i-1] = num%B.digits[m];
			C[0].digits[k++] = num/B.digits[m];
		 }
		 if(A.digits[0] >= B.digits[m])	{
		    C[0].digits[k++] = A.digits[0]/B.digits[m];
		    C[1].digits[0] = A.digits[0]%B.digits[m];
		 }
		 else{
		 	C[1].digits[0] = A.digits[0];
		 }   	
		 C[0].length = k;
 
	 }
	 else if(m>n){
	 	for(i=n-1;i>=0;i--)
	 		C[1].digits[l++]=A.digits[i];
	 	C[1].length = l;	
	 }

	



	//return C;
}

BigInteger multiply (BigInteger A, BigInteger B) {
	int	i, j;
	BigInteger P = init();	

	BigInteger C = init();
	/* C will accumulate the sum of partial products.  It's initially 0. */

 	//make_int (C, 0);

	/* for each digit in A... */

	for (i=0; i<N; i++) {
		/* multiply B by digit A[i] */

		multiply_one_digit (B.digits,  P.digits , A.digits[i]);

		/* shift the partial product left i bytes */

		shift_left (P.digits, i);

		/* add result to the running sum */

		C = add (C, P);
	}

	return C;
}


void multiply_one_digit (int A[], int B[], int n) {
	int	i, carry;

	/* no extra overflow to add yet */

	carry = 0;

	/* for each digit, starting with least significant... */

	for (i=0; i<N; i++) {

		/* multiply the digit by n, putting the result in B */

		B[i] = n * A[i];

		/* add in any overflow from the last digit */

		B[i] += carry;

		/* if this product is too big to fit in a digit... */

		if (B[i] >= BASE) {

			/* handle the overflow */

			carry = B[i] / BASE;
			B[i] %= BASE;
		} 
		else

			/* no overflow */

			carry = 0;
	}

	if (carry) printf ("overflow in multiplication!\n");
}


void shift_left (int A[], int n) {
	int	i;

	/* going from left to right, move everything over to the
	 * left n spaces
	 */
	for (i=N-1; i>=n; i--) A[i] = A[i-n];

	/* fill the last n digits with zeros */

	while (i >= 0) A[i--] = 0;
}

int enhanced_euclid(int p0, int p1){
	int temp;
	int l,m,t,s,tos,flag=0,i,b=0;
	l=p0;
	m=p1;
	int stack[100];
	tos=0;
	do{
		t = l/m;
		stack[tos++]=t;
		s = l - m*t;
		b=1-b;
		l=m;
		m=s;
		if(s==0){
			flag=1;
			break;
		}

		
	}while(s!=1);
		
	if(flag){
		printf("Inverse doesn\'t exist\n");
		return -1;
	}
	
	if(b==0)
		m=1;
	else
		m=-1;
	s=0;
	printf("m %d \n",m );
	for(i=tos-1;i>=0;i--){
	
		l = m*stack[i]+s;
		s=m;
		m=l;
		printf(" s %d   m %d   stack %d \n",s,m, stack[i]);
	}
	return l;	


}



int mul_inv(int a, int b)
{
        int t, nt, r, nr, q, tmp;
        if (b < 0) b = -b;
        if (a < 0) a = b - (-a % b);
        t = 0;  nt = 1;  r = b;  nr = a % b;
        while (nr != 0) {
          q = r/nr;
          tmp = nt;  nt = t - q*nt;  t = tmp;
          //  printf("%d %d %d\n", nt, t, q );
          tmp = nr;  nr = r - q*nr;  r = tmp;
           // printf("%d %d %d\n", nr, r, q );
        }
        if (r > 1) return -1;  /* No inverse */
        if (t < 0) t += b;
        return t;
}


int * intdup(int const * src, int *dest, size_t len)
{
   int * p = malloc(len * sizeof(int));
   memcpy(p, src, len * sizeof(int));
   return p;
}

int main(int argc , char *argv[]){


// 	printf("%d\n", mul_inv(4 , 5));
//	printf("%d\n", enhanced_euclid(5, 9));
//	printf("%d\n", enhanced_euclid(5, 4));

//	printf("%d\n", enhanced_euclid(9, 7));
//	printf("%d\n", enhanced_euclid(31415926, 27182845));
	int i;
	BigInteger result;
	BigInteger qr[2];
	qr[0] = init();
	qr[1] = init();
	BigInteger d1, d2;
	int len_d1, len_d2, len_r, len_q;
	int dig1_int[N], dig2_int[N];
	int sign_dig1=1,sign_dig2=1;

	char dig1[] = "45154548787";

	char dig2[] = "545454545454545454";

	len_d1 = strlen(dig1);

	len_d2 = strlen(dig2);
	
	rep(i,N)
		dig1_int[i]=0;

	rep(i,N)
		dig2_int[i]=0;	


	rep(i, len_d1){
		dig1_int[i] = dig1[len_d1-i-1] - '0';
	}

	rep(i, len_d2){
		dig2_int[i] = dig2[len_d2-i-1] - '0';
	}

	d1.sign = sign_dig1;
	d1.sign = sign_dig2;

	d1.length = len_d1;
	d2.length = len_d2;

	memcpy(d1.digits , dig1_int, N*sizeof(int));
	memcpy(d2.digits , dig2_int, N*sizeof(int));

	//result = add(d1, d2);
	//result = multiply(d1, d2);
	//result = subtract(d1, d2);
	//rep(i, N)
	//	result.digits[i]=0;
	/*len_r = result.length;
	rep(i, len_r)
		printf("%d", result.digits[len_r-1-i]);
	printf("\n");
	printf("%d\n", result.length );
*/
	knuth_divide(d1, d2, qr);
	result = qr[0];
	//printf("%d\n",qr[0].digits[0] );
	len_q = result.length;
	printf("%d\n", len_q);

	rep(i, len_q)
		printf("%d",result.digits[i]);
	printf("\n");

	len_r = qr[1].length;
	rep(i, len_r)
		printf("%d",qr[1].digits[i]);
	printf("\n");
	//int b = check_max(d1, d2);
	//printf(" max result %d\n",b);
	 
	return 0;
}
