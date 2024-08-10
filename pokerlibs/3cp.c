#include <stdio.h>
#include <string.h>

/*   AKQs-A23s    12 SFL       1- 12
 *   AAA -222     13 3OAK     13- 25
 *   AKJo-A23o    12 Str8     26- 37
 *   AKJs-235s   274 Flush    38-311
 *   AAK -223    156 Pair    312-467
 *   AKJo-235o   274 Nothing 468-741
 */

/* offset gives the base hand number for a given high card offset by
 * the nonstraight base. For example, AKJ is the highest ace-high,
 * and it has hand number 0 + nonstraightbase. The highest 5-high hand
 * is 542, which is 272 + nonstraightbase.
 */
static const int offset[] = {0,64,118,162,197,224,244,258,267,272,274};

/* unique3 computes a hand number for three distinctly-ranked cards a,b,c.
 * straightbase represents the smallest rank of a straight, nonstraightbase,
 * the smallest hand rank of a non-straight. This means unique3 is usable
 * for both flush and non-flush hands.
 */
static int unique3(int a, int b, int c, int straightbase,int nonstraightbase) {
  int tmp, i;

  /* first use recursion to sort out a,b,c */
  if (a < b)
    return unique3(b,a,c,straightbase,nonstraightbase);
  if (a < c)
    return unique3(c,a,b,straightbase,nonstraightbase);
  if (b < c)
    return unique3(a,c,b,straightbase,nonstraightbase);

  /* check for straights other than the wheel */
  if (a == (b+1) && b == (c+1))
    return (11-c)+straightbase;

  /* check for wheel */
  if (a == 12 && b == 1 && !c)
    return 12+straightbase;

  /* non-straight */
  tmp = offset[12-a];

  /* This is a bit of fancy math that increments the eventual hand number
   * (represented here by tmp) based on the second kicker. Note that
   * as the second kicker goes down, fewer and fewer hands are represented
   * by that reduction, because a small second-kicker goes with fewer potential
   * third-kickers. There's a special case in the loop for the highest possible
   * second-kicker, because, for example, in ace-high, there are equally many
   * AKx as AQx (since AKQ doesn't reach this code). Thus for the highest 
   * possible second-kicker (i==a-1), we tmp--;
   */
  for (i=a-1;i>b;i--) {
    tmp += i;
    if (i==a-1)
      tmp--;
  }

  /* Now use the third-kicker. Again, a special case for top-two in order:
   * AKJ would add 1 using the (b-1)-c rule, but it should add 0, hence
   * when a==b+1 (in order top-two), we decrement tmp to make up for it
   */
  if (a==b+1) tmp--;
  tmp += (b-1)-c;
  return tmp+nonstraightbase+1;
}

/* Handle any pairs. Since there are no pairs with flush, no need to worry
 * about flushes.
 */
static int pairbetter3(int a, int b, int c) {
  /* Three of a kind */
  if ((a == b) && (b == c))
    return 24-a;

  /* Reorder so that the pair is a==b with kicker c */
  if (b == c)
    return pairbetter3(b,c,a);
  if (a == c)
    return pairbetter3(a,c,b);

  /* There are 12 possible kickers for each pair. Since the paired number is
   * not a possible kicker, when the kicker is higher than the pair, we take
   * 12-kicker (thereby counting down each card Ace (0), King (1), ...),
   * and when the kicker is lower than the pair, we take 11-kicker, to
   * skip the case where kicker = a. For example, if we have a pair of queens,
   * then the first case applies to Ace (12-12 = 0) and King (12-11=1), and 
   * the second case applies to Jack (11-9=2), Ten (11-8=3), and below.
   */
  if (a > c) {
    return 311 + 12*(12-a) + (11-c);
  } else {
    return 311 + 12*(12-a) + (12-c);
  }
}

/* Putting it all together... */
int threecardnum(int a, int b, int c) {
  /* sa, sb, sc are suit, ra, rb, rc are rank */
  int sa = a/13;
  int ra = a%13;
  int sb = b/13;
  int rb = b%13;
  int sc = c/13;
  int rc = c%13;

  /* check for flush */
  if (sa == sb && sb == sc) {
    return unique3(ra,rb,rc,-1,36)+1;
  }

  /* check for pairs */
  if (ra == rb || rb == rc || ra == rc)
    return pairbetter3(ra,rb,rc)+1;

  /* non-flush unique (straight or nothing) */
  return unique3(ra, rb, rc, 24, 466)+1;
}


