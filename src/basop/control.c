/*
  ===========================================================================
   File: CONTROL.C                                       v.2.3 - 30.Nov.2009
  ===========================================================================

  ITU-T  STL  BASIC  OPERATORS

  CONTROL FLOW OPERATOR INTERNAL VARIABLE DECLARATIONS

   History:
      07 Nov 04   v2.0  Incorporation of new 32-bit / 40-bit / control
                        operators for the ITU-T Standard Tool Library as 
                        described in Geneva, 20-30 January 2004 WP 3/16 Q10/16
                        TD 11 document and subsequent discussions on the
                        wp3audio@yahoogroups.com email reflector.
      March 06    v2.1  Changed to improve portability.
   
  ============================================================================
*/

#include "stl.h"

#ifdef WMOPS
int funcId_where_last_call_to_else_occurred;
long funcid_total_wmops_at_last_call_to_else;
int call_occurred = 1;
#endif

#ifdef CONTROL_CODE_OPS

Flag LT_16 (Word16 var1, Word16 var2) {
  Flag F_ret = 0;
  
  if( var1 < var2 )
  {
    F_ret = 1;
  }
#if (WMOPS)
  multiCounter[currCounter].LT_16++;
#endif
  return F_ret;
}

Flag GT_16 (Word16 var1, Word16 var2) {
  Flag F_ret = 0;
  
  if( var1 > var2 )
  {
    F_ret = 1;
  }
#if (WMOPS)
  multiCounter[currCounter].GT_16++;
#endif  
  return F_ret;
}

Flag LE_16 (Word16 var1, Word16 var2) {
  Flag F_ret = 0;
  
  if (var1 <= var2) {
    F_ret = 1;
  }
#if (WMOPS)
  multiCounter[currCounter].LE_16++;
#endif  
  return F_ret;
}

Flag GE_16 (Word16 var1, Word16 var2) {
  Flag F_ret = 0;
  
  if (var1 >= var2) {
    F_ret = 1;
  }
#if (WMOPS)
  multiCounter[currCounter].GE_16++;
#endif  
  return F_ret;
}

Flag EQ_16 (Word16 var1, Word16 var2) {
  Flag F_ret = 0;
  
  if (var1 == var2) {
    F_ret = 1;
  }
#if (WMOPS)
  multiCounter[currCounter].EQ_16++;
#endif  
  return F_ret;
}

Flag NE_16 (Word16 var1, Word16 var2) {
  Flag F_ret = 0;
  
  if (var1 != var2) {
    F_ret = 1;
  }
#if (WMOPS)
  multiCounter[currCounter].NE_16++;
#endif  
  return F_ret;
}

Flag LT_32 (Word32 L_var1, Word32 L_var2) {
  Flag F_ret = 0;
  
  if (L_var1 < L_var2) {
    F_ret = 1;
  }
#if (WMOPS)
  multiCounter[currCounter].LT_32++;
#endif  
  return F_ret;
}

Flag GT_32 (Word32 L_var1, Word32 L_var2) {
  Flag F_ret = 0;
  
  if (L_var1 > L_var2) {
    F_ret = 1;
  }
#if (WMOPS)
  multiCounter[currCounter].GT_32++;
#endif  
  return F_ret;
}

Flag LE_32 (Word32 L_var1, Word32 L_var2) {
  Flag F_ret = 0;
  
  if (L_var1 <= L_var2) {
    F_ret = 1;
  }
#if (WMOPS)
  multiCounter[currCounter].LE_32++;
#endif
  return F_ret;
}

Flag GE_32 (Word32 L_var1, Word32 L_var2) {
  Flag F_ret = 0;
  
  if (L_var1 >= L_var2) {
    F_ret = 1;
  }
#if (WMOPS)
  multiCounter[currCounter].GE_32++;
#endif  
  return F_ret;
}

Flag EQ_32 (Word32 L_var1, Word32 L_var2) {
  Flag F_ret = 0;
  
  if (L_var1 == L_var2) {
    F_ret = 1;
  }
#if (WMOPS)
  multiCounter[currCounter].EQ_32++;
#endif
  return F_ret;
}

Flag NE_32 (Word32 L_var1, Word32 L_var2) {
  Flag F_ret = 0;
  
  if (L_var1 != L_var2) {
    F_ret = 1;
  }
#if (WMOPS)
  multiCounter[currCounter].NE_32++;
#endif  
  return F_ret;
}

Flag LT_64 (Word64 L64_var1, Word64 L64_var2) {
  Flag F_ret = 0;
  
  if (L64_var1 < L64_var2) {
    F_ret = 1;
  }
#if (WMOPS)
  multiCounter[currCounter].LT_64++;
#endif  
  return F_ret;
}

Flag GT_64 (Word64 L64_var1, Word64 L64_var2) {
  Flag F_ret = 0;
  
  if (L64_var1 > L64_var2) {
    F_ret = 1;
  }
#if (WMOPS)
  multiCounter[currCounter].GT_64++;
#endif  
  return F_ret;
}

Flag LE_64 (Word64 L64_var1, Word64 L64_var2) {
  Flag F_ret = 0;
  
  if (L64_var1 <= L64_var2) {
    F_ret = 1;
  }
#if (WMOPS)
  multiCounter[currCounter].LE_64++;
#endif  
  return F_ret;
}
Flag GE_64 (Word64 L64_var1, Word64 L64_var2) {
  Flag F_ret = 0;
  
  if (L64_var1 >= L64_var2) {
    F_ret = 1;
  }
#if (WMOPS)
  multiCounter[currCounter].GE_64++;
#endif  
  return F_ret;
}

Flag EQ_64 (Word64 L64_var1, Word64 L64_var2) {
  Flag F_ret = 0;
  
  if (L64_var1 == L64_var2) {
    F_ret = 1;
  }
#if (WMOPS)
  multiCounter[currCounter].EQ_64++;
#endif  
  return F_ret;
}
Flag NE_64 (Word64 L64_var1, Word64 L64_var2) {
  Flag F_ret = 0;
  
  if (L64_var1 != L64_var2) {
    F_ret = 1;
  }
#if (WMOPS)
  multiCounter[currCounter].NE_64++;
#endif  
  return F_ret;
}

#endif /* #ifdef CONTROL_CODE_OPS */


/* end of file */
