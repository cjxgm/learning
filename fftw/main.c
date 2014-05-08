
#include <stdio.h>
#include <math.h>
#include "fft.h"

int main()
{
	int n;
	printf("ninput: ");
	scanf("%d", &n);

	double inputs[n];
	printf("inputs: ");
	for (int i=0; i<n; i++) scanf("%lf", &inputs[i]);

	fft(inputs, n);
	printf(" fft(inputs, ninput):");
	for (int i=0; i<n; i++) printf(" % 2.2f", inputs[i]);
	putchar('\n');

	ifft(inputs, n);
	printf("ifft(inputs, ninput):");
	for (int i=0; i<n; i++) printf(" % 2.2f", inputs[i]);
	putchar('\n');

	return 0;
}

