#include <stdio.h>

/* Hand numbers
 * 4OAK       1 to   13    (AAAA-2222)
 ****** Str8Fl    14 to   24    (JKQAs-A234s)
 * 3OAK      25 to  180    (AAAK-2223)
 ****** Flush    181 to  884    (AKQTs-2346s)
 * Str8     885 to  895    (JKQAo-A234o)
 * TwoPr    896 to  973    (AAKK-3322)
 * Pair     974 to 1831    (AAKQ-2234)
 * Nothing 1832 to 2535    (AKQTo-2346o)
 */

#define UNDEFINED_HAND 2536

/* cumulative sum from i=0 to index of 12-i. */
static const int cumsum[]={0, 12, 23, 33, 42, 50, 57, 63, 68, 72, 75, 77, 78};

int *lookuptab_4cp = NULL;
void init_hand_nums_4cp() {
  int a,b,c,d, handnum, basenum;
  if (lookuptab_4cp) return;
  lookuptab_4cp = malloc(sizeof(int) * 28561);
  basenum = 1832;

  /* First, populate all the hands that are non-nothing */
  for (a=0;a<13;a++) {
    for (b=a;b<13;b++) {
      for (c=b;c<13;c++) {
	for (d=c;d<13;d++) {
	  handnum = -1;
	  if (a==b && b==c && c==d) {
	    /* four of a kind */
	    handnum = 13-a;
	  } else if (a == b && b == c) {
	    /* Three of a kind, d is the kicker, and d > c.
	     * 12-c tells us the first card, 11-d is the kicker number
	     */
	    handnum = (12-c)*12+(12-d) + 25;
	    //printf("1t%ck%c %d %d\n", foo[a], foo[d], 12-c, 12-d);
	  } else if (b == c && c == d) {
	    /* Three of a kind, a is the kicker, and a < b
	     * 12-c tells us the first card, 12-a is the kicker number
	     */
	    handnum = (12-c)*12+(11-a) + 25;
	    //printf("2t%ck%c %d %d\n", foo[b], foo[a], 12-c, 11-a);
	  } else if (a+1 == b && b+1 == c && c+1 == d) {
	    /* d high straight */
	    handnum = (9-a) + 885;
	  } else if (a == 0 && b == 1 && c == 2 && d == 12) {
	    /* wheel straight */
	    handnum = 895;
	  } else if (a == b && c == d) {
	    /* two pair */
	    handnum = cumsum[12-d] + (d-a) + 895;
	  } else if (c == d) {
	    /* pair, first kicker b < c, second kicker a < b */
	    handnum = (12-c)*66 + cumsum[11-b] - 2*(11-b) + (10-a) + 974;
	  } else if (b == c) {
	    /* pair, first kicker d > c, second kicker a < c */
	    handnum = (12-c)*66 + cumsum[12-d] - 2*(12-d) + (10-a) + 974;
	  } else if (a == b) {
	    /* pair, first kicker d, second kicker c */
	    handnum = (12-b)*66 + cumsum[12-d] - 2*(12-d) + (11-c) + 974;
	  }

	  /* insert all 4! = 24 permutations into the LUT */
	  lookuptab_4cp[13*13*13*a+13*13*b+13*c+d] = handnum;
	  lookuptab_4cp[13*13*13*a+13*13*b+13*d+c] = handnum;
	  lookuptab_4cp[13*13*13*a+13*13*c+13*b+d] = handnum;
	  lookuptab_4cp[13*13*13*a+13*13*c+13*d+b] = handnum;
	  lookuptab_4cp[13*13*13*a+13*13*d+13*b+c] = handnum;
	  lookuptab_4cp[13*13*13*a+13*13*d+13*c+b] = handnum;

	  lookuptab_4cp[13*13*13*b+13*13*a+13*c+d] = handnum;
	  lookuptab_4cp[13*13*13*b+13*13*a+13*d+c] = handnum;
	  lookuptab_4cp[13*13*13*b+13*13*c+13*a+d] = handnum;
	  lookuptab_4cp[13*13*13*b+13*13*c+13*d+a] = handnum;
	  lookuptab_4cp[13*13*13*b+13*13*d+13*a+c] = handnum;
	  lookuptab_4cp[13*13*13*b+13*13*d+13*c+a] = handnum;

	  lookuptab_4cp[13*13*13*c+13*13*b+13*a+d] = handnum;
	  lookuptab_4cp[13*13*13*c+13*13*b+13*d+a] = handnum;
	  lookuptab_4cp[13*13*13*c+13*13*a+13*b+d] = handnum;
	  lookuptab_4cp[13*13*13*c+13*13*a+13*d+b] = handnum;
	  lookuptab_4cp[13*13*13*c+13*13*d+13*b+a] = handnum;
	  lookuptab_4cp[13*13*13*c+13*13*d+13*a+b] = handnum;

	  lookuptab_4cp[13*13*13*d+13*13*b+13*c+a] = handnum;
	  lookuptab_4cp[13*13*13*d+13*13*b+13*a+c] = handnum;
	  lookuptab_4cp[13*13*13*d+13*13*c+13*b+a] = handnum;
	  lookuptab_4cp[13*13*13*d+13*13*c+13*a+b] = handnum;
	  lookuptab_4cp[13*13*13*d+13*13*a+13*b+c] = handnum;
	  lookuptab_4cp[13*13*13*d+13*13*a+13*c+b] = handnum;
	}
      }
    }
  }

  /* Then go through all the nothing hands in reverse order of value,
   * and set their hand numbers to ever-increasing values, starting
   * from the basenum initialized above.
   */
  for (a=12;a>=0;a--) {
    for (b=a-1;b>=0;b--) {
      for (c=b-1;c>=0;c--) {
	for (d=c-1;d>=0;d--) {
	  if (lookuptab_4cp[13*13*13*a+13*13*b+13*c+d] == -1) {
	    handnum = basenum++;
	    lookuptab_4cp[13*13*13*a+13*13*b+13*c+d] = handnum;
	    lookuptab_4cp[13*13*13*a+13*13*b+13*d+c] = handnum;
	    lookuptab_4cp[13*13*13*a+13*13*c+13*b+d] = handnum;
	    lookuptab_4cp[13*13*13*a+13*13*c+13*d+b] = handnum;
	    lookuptab_4cp[13*13*13*a+13*13*d+13*b+c] = handnum;
	    lookuptab_4cp[13*13*13*a+13*13*d+13*c+b] = handnum;

	    lookuptab_4cp[13*13*13*b+13*13*a+13*c+d] = handnum;
	    lookuptab_4cp[13*13*13*b+13*13*a+13*d+c] = handnum;
	    lookuptab_4cp[13*13*13*b+13*13*c+13*a+d] = handnum;
	    lookuptab_4cp[13*13*13*b+13*13*c+13*d+a] = handnum;
	    lookuptab_4cp[13*13*13*b+13*13*d+13*a+c] = handnum;
	    lookuptab_4cp[13*13*13*b+13*13*d+13*c+a] = handnum;

	    lookuptab_4cp[13*13*13*c+13*13*b+13*a+d] = handnum;
	    lookuptab_4cp[13*13*13*c+13*13*b+13*d+a] = handnum;
	    lookuptab_4cp[13*13*13*c+13*13*a+13*b+d] = handnum;
	    lookuptab_4cp[13*13*13*c+13*13*a+13*d+b] = handnum;
	    lookuptab_4cp[13*13*13*c+13*13*d+13*b+a] = handnum;
	    lookuptab_4cp[13*13*13*c+13*13*d+13*a+b] = handnum;
	    
	    lookuptab_4cp[13*13*13*d+13*13*b+13*c+a] = handnum;
	    lookuptab_4cp[13*13*13*d+13*13*b+13*a+c] = handnum;
	    lookuptab_4cp[13*13*13*d+13*13*c+13*b+a] = handnum;
	    lookuptab_4cp[13*13*13*d+13*13*c+13*a+b] = handnum;
	    lookuptab_4cp[13*13*13*d+13*13*a+13*b+c] = handnum;
	    lookuptab_4cp[13*13*13*d+13*13*a+13*c+b] = handnum;
	  }
	}
      }
    }
  }

#if 0
  for (a=0;a<13;a++) {
    for (b=0;b<13;b++) {
      for (c=0;c<13;c++) {
	for (d=0;d<13;d++) {
	  num = 13*13*13*a+13*13*b+13*c+d;
	  printf("%6d %2d %2d %2d %2d - %6d %6d\n", lookuptab_4cp[num],a,b,c,d,num,
		 lookuptab_4cp[num]);
	}
      }
    }
  }
#endif
}

/* Str8Fl    14 to   24    (JKQAs-A234s)
 * Flush    181 to  884    (AKQTs-2346s)
 *
 * Str8     885 to  895    (JKQAo-A234o)
 * Nothing 1832 to 2535    (AKQTo-2346o)
 *
 * if <896, subtract  871
 * else,    subtract 1651
 */

int flushable(int handnum) {
  if (handnum >= 885 && handnum <= 895) return handnum-871;
  if (handnum >= 1832) return handnum-1651;
  return 0;
}

int handnum4cp(const int *cnum, const int *rnum, const int *snum) {
  int num = lookuptab_4cp[13*13*13*rnum[0]+13*13*rnum[1]+13*rnum[2]+rnum[3]];

  /* is it flush? */
  if (snum[0] != snum[1] || snum[1] != snum[2] || snum[2] != snum[3])
    return num;

  return flushable(num);
}

int handnum4(int a, int b, int c, int d) {
  int ra = a%13;
  int sa = a/13;
  int rb = b%13;
  int sb = b/13;
  int rc = c%13;
  int sc = c/13;
  int rd = d%13;
  int sd = d/13;

  int num = lookuptab_4cp[13*13*13*ra+13*13*rb+13*rc+rd];
  if (sa != sb || sb != sc || sc != sd)
    return num;
  return flushable(num);
}
  
