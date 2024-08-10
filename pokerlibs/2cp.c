int twocard_ronly(int a, int b) {
  if (a == b)
    return 13-a;
  if (a < b)
    return twocard_ronly(b, a);
  return 91-a*(a+1)/2+(a-b);
}

int twocardnum(int a, int b) {
  int sa = a/13;
  int ra = a%13;
  int sb = b/13;
  int rb = b%13;
  int hand = twocard_ronly(ra,rb);
  if (sa == sb) return hand;
  if (ra == rb) return hand;
  return 78+hand;
}
