{
  print "0x6B21"
  printf("0x%00x\n", $1)
  for (i=0;i<$1;i++)
     print i%2? "0x007F":"0x0081"
}
