#include <stdio.h>
#include <gmp.h>
#include <stdlib.h>

static mpz_t ONE;

void stirling(mpz_t * rop, mpz_t n, mpz_t k)
{
	unsigned long nl = mpz_get_ui(n);
	mpz_t acc; mpz_t kj; mpz_t t1; mpz_t t2; mpz_t i;
	mpz_inits(i, acc, kj, t1, t2, NULL);
	mpz_set_ui(kj, 1U);

	for (; mpz_cmp(i, k) <= 0; mpz_add(i, i, ONE)) {
		mpz_sub(t1, k, i);
		mpz_pow_ui(t2, t1, nl);

		if (mpz_even_p(i)) {
			mpz_addmul(acc, kj, t2);
		} else {
			mpz_submul(acc, kj, t2);
		}

		mpz_add(t2, i, ONE);
		mpz_mul(kj, kj, t1);
		mpz_divexact(kj, kj, t2);
	}
	
	mpz_fac_ui(t1, mpz_get_ui(k));
	mpz_divexact(*rop, acc, t1);

	mpz_clears(i, acc, kj, t1, t2, NULL);
}	


void b(mpz_t * rop, mpz_t x, mpz_t r) {
	unsigned long rl = mpz_get_ui(r);
	mpz_t k; mpz_t t1; mpz_t t2; mpz_t t3; mpz_t pk;
	mpz_inits(k, t1, t2, t3, pk, NULL);
	mpz_set_ui(k, 1U);
	mpz_set(pk, x);

	for (; mpz_cmp(k, r) <= 0; mpz_add(k, k, ONE)) {
		mpz_sub(t1, x, k);
		mpz_pow_ui(t2, t1, rl);
		mpz_mul(t2, t2, pk);
		stirling(&t3, r, k);
		mpz_mul(t3, t3, t2);
		mpz_add(*rop, *rop, t3);
		mpz_mul(pk, pk, t1);
	}
	mpz_clears(k, t1, t2, t3, pk, NULL);
}

void calculate(mpq_t * rop, mpz_t x, mpz_t r)
{
	mpz_t t1; mpz_t t2;
	mpz_inits(t1, t2, NULL);

	mpz_mul_ui(t2, r, 2);
	mpz_pow_ui(t2, x, mpz_get_ui(t2));
	b(&t1, x, r);
	
	mpz_ptr num = mpq_numref(*rop);
	mpz_ptr den = mpq_denref(*rop);

	mpz_set(num, t1);
	mpz_set(den, t2);
	mpq_canonicalize(*rop);

	mpz_clears(t1, t2, NULL);
}	

int main()
{
	mpf_set_default_prec(4096);
	mpq_t result;
	mpz_t n; mpz_t k;
	
	mpq_init(result);
	mpz_inits(n, k, ONE, NULL);
	mpz_set_ui(ONE, 1U);
	mpz_set_ui(n, (unsigned long)1 << 32);
	mpz_set_ui(k, 1 << 10);

	calculate(&result, n, k);
	
	mpf_t f;
	mpf_init(f);
	mpf_set_q(f, result);

	size_t num = mpz_sizeinbase(mpq_numref(result), 10);
	size_t den = mpz_sizeinbase(mpq_denref(result), 10);

	gmp_printf("%.*Ff\n", 128, f, 128);
	gmp_printf("10^%ld / 10^%ld\n", num, den);

	mpz_clears(ONE, n, k, NULL);
	mpq_clear(result);
	mpf_clear(f);
	return 0;
}
