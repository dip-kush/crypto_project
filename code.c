#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<math.h>

#define BASE 10

#define N 128

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

void add (int A[], int B[], int C[]) {
	int	i, carry, sum;

	/* no carry yet */

	carry = 0;

	/* go from least to most significant digit */

	for (i=0; i<N; i++) {

		/* the i'th digit of C is the sum of the
		 * i'th digits of A and B, plus any carry
		 */
		sum = A[i] + B[i] + carry;

		/* if the sum exceeds the base, then we have a carry. */

		if (sum >= BASE) {

			carry = 1;

			/* make sum fit in a digit (same as sum %= BASE) */

			sum -= BASE;
		} else
			/* otherwise no carry */

			carry = 0;

		/* put the result in the sum */

		C[i] = sum;
	}

	/* if we get to the end and still have a carry, we don't have
	 * anywhere to put it, so panic! 
	 */
	if (carry) printf ("overflow in addition!\n");
}

void multiply (int A[], int B[], int C[]) {
	int	i, j, P[N];

	/* C will accumulate the sum of partial products.  It's initially 0. */

	make_int (C, 0);

	/* for each digit in A... */

	for (i=0; i<N; i++) {
		/* multiply B by digit A[i] */

		multiply_one_digit (B, P, A[i]);

		/* shift the partial product left i bytes */

		shift_left (P, i);

		/* add result to the running sum */

		add (C, P, C);
	}
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
		} else

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




int main(int argc , char *argv[]){
 	printf("%d\n", mul_inv(4 , 5));
	printf("%d\n", enhanced_euclid(5, 9));
		printf("%d\n", enhanced_euclid(5, 4));

//	printf("%d\n", enhanced_euclid(9, 7));
//	printf("%d\n", enhanced_euclid(31415926, 27182845));
	return 0;
}
