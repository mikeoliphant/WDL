#ifndef _WDL_DENORMAL_H_
#define _WDL_DENORMAL_H_

typedef struct 
{ 
  #ifdef __ppc__ // todo: other big endian platforms...
    unsigned int hw; 
    unsigned int lw;
  #else
    unsigned int lw; 
    unsigned int hw;
  #endif
} WDL_DenormalTwoInts;

typedef union { double fl; WDL_DenormalTwoInts w; } WDL_DenormalDoubleAccess;
typedef union { float fl; unsigned int w; } WDL_DenormalFloatAccess;


// note: the _aggressive versions filter out anything less than around 1.0e-16 or so (approximately) to 0.0, including -0.0 (becomes 0.0)
// note: new! the _aggressive versions also filter inf and NaN to 0.0

// note: new!! FTZ/DAZ support, see WDL_DENORMAL_FTZ, WDL_DENORMAL_DAZ, and WDL_DenormalMode
// optionally #define WDL_DENORMAL_BYPASS to bypass filter/fix functions, for easy switching between filter/fix and FTZ/DAZ

#ifdef __cplusplus
#define WDL_DENORMAL_INLINE inline
#elif defined(_MSC_VER)
#define WDL_DENORMAL_INLINE __inline
#else
#define WDL_DENORMAL_INLINE
#endif

#define WDL_DENORMAL_DOUBLE_HW(a) (((const WDL_DenormalDoubleAccess*)(a))->w.hw)
#define WDL_DENORMAL_DOUBLE_LW(a) (((const WDL_DenormalDoubleAccess*)(a))->w.lw)
#define WDL_DENORMAL_FLOAT_W(a) (((const WDL_DenormalFloatAccess*)(a))->w)

#define WDL_DENORMAL_DOUBLE_HW_NC(a) (((WDL_DenormalDoubleAccess*)(a))->w.hw)
#define WDL_DENORMAL_DOUBLE_LW_NC(a) (((WDL_DenormalDoubleAccess*)(a))->w.lw)
#define WDL_DENORMAL_FLOAT_W_NC(a) (((WDL_DenormalFloatAccess*)(a))->w)

#define WDL_DENORMAL_DOUBLE_AGGRESSIVE_CUTOFF 0x3cA00000 // 0x3B8000000 maybe instead? that's 10^-5 smaller or so
#define WDL_DENORMAL_FLOAT_AGGRESSIVE_CUTOFF 0x25000000


#ifndef WDL_DENORMAL_BYPASS // define when already flushing denormals to zero via fesetenv or _controlfp_s

static double WDL_DENORMAL_INLINE denormal_filter_double(double a)
{
  return (WDL_DENORMAL_DOUBLE_HW(&a)&0x7ff00000) ? a : 0.0;
}

static double WDL_DENORMAL_INLINE denormal_filter_double2(double a)
{
  return ((WDL_DENORMAL_DOUBLE_HW(&a)+0x100000)&0x7ff00000) > 0x100000 ? a : 0.0;
}

static double WDL_DENORMAL_INLINE denormal_filter_double_aggressive(double a)
{
  return ((WDL_DENORMAL_DOUBLE_HW(&a)+0x100000)&0x7ff00000) >= WDL_DENORMAL_DOUBLE_AGGRESSIVE_CUTOFF ? a : 0.0;
}

static float WDL_DENORMAL_INLINE denormal_filter_float(float a)
{
  return (WDL_DENORMAL_FLOAT_W(&a)&0x7f800000) ? a : 0.0f;
}

static float WDL_DENORMAL_INLINE denormal_filter_float2(float a)
{
  return ((WDL_DENORMAL_FLOAT_W(&a)+0x800000)&0x7f800000) > 0x800000 ? a : 0.0f; 
}


static float WDL_DENORMAL_INLINE denormal_filter_float_aggressive(float a)
{
  return ((WDL_DENORMAL_FLOAT_W(&a)+0x800000)&0x7f800000) >= WDL_DENORMAL_FLOAT_AGGRESSIVE_CUTOFF ? a : 0.0f; 
}
static void WDL_DENORMAL_INLINE denormal_fix_double(double *a)
{
  if (!(WDL_DENORMAL_DOUBLE_HW(a)&0x7ff00000)) *a=0.0;
}

static void WDL_DENORMAL_INLINE denormal_fix_double_aggressive(double *a)
{
  if (((WDL_DENORMAL_DOUBLE_HW(a)+0x100000)&0x7ff00000) < WDL_DENORMAL_DOUBLE_AGGRESSIVE_CUTOFF) *a=0.0;
}

static void WDL_DENORMAL_INLINE denormal_fix_float(float *a)
{
  if (!(WDL_DENORMAL_FLOAT_W(a)&0x7f800000)) *a=0.0f;
}
static void WDL_DENORMAL_INLINE denormal_fix_float_aggressive(float *a)
{
  if (((WDL_DENORMAL_FLOAT_W(a)+0x800000)&0x7f800000) < WDL_DENORMAL_FLOAT_AGGRESSIVE_CUTOFF) *a=0.0f;
}



#ifdef __cplusplus // automatic typed versions (though one should probably use the explicit versions...


static double WDL_DENORMAL_INLINE denormal_filter(double a)
{
  return (WDL_DENORMAL_DOUBLE_HW(&a)&0x7ff00000) ? a : 0.0;
}
static double WDL_DENORMAL_INLINE denormal_filter_aggressive(double a)
{
  return ((WDL_DENORMAL_DOUBLE_HW(&a)+0x100000)&0x7ff00000) >= WDL_DENORMAL_DOUBLE_AGGRESSIVE_CUTOFF ? a : 0.0;
}

static float WDL_DENORMAL_INLINE denormal_filter(float a)
{
  return (WDL_DENORMAL_FLOAT_W(&a)&0x7f800000) ? a : 0.0f;
}

static float WDL_DENORMAL_INLINE denormal_filter_aggressive(float a)
{
  return ((WDL_DENORMAL_FLOAT_W(&a)+0x800000)&0x7f800000) >= WDL_DENORMAL_FLOAT_AGGRESSIVE_CUTOFF ? a : 0.0f;
}

static void WDL_DENORMAL_INLINE denormal_fix(double *a)
{
  if (!(WDL_DENORMAL_DOUBLE_HW(a)&0x7ff00000)) *a=0.0;
}
static void WDL_DENORMAL_INLINE denormal_fix_aggressive(double *a)
{
  if (((WDL_DENORMAL_DOUBLE_HW(a)+0x100000)&0x7ff00000) < WDL_DENORMAL_DOUBLE_AGGRESSIVE_CUTOFF) *a=0.0;
}
static void WDL_DENORMAL_INLINE denormal_fix(float *a)
{
  if (!(WDL_DENORMAL_FLOAT_W(a)&0x7f800000)) *a=0.0f;
}
static void WDL_DENORMAL_INLINE denormal_fix_aggressive(float *a)
{
  if (((WDL_DENORMAL_FLOAT_W(a)+0x800000)&0x7f800000) < WDL_DENORMAL_FLOAT_AGGRESSIVE_CUTOFF) *a=0.0f;
}



#endif // cplusplus versions



#else // WDL_DENORMAL_BYPASS

#define denormal_filter_double(a) (a)
#define denormal_filter_double2(a) (a)
#define denormal_filter_double_aggressive(a) (a)
#define denormal_filter_float(a) (a)
#define denormal_filter_float2(a) (a)
#define denormal_filter_float_aggressive(a) (a)
#define denormal_fix_double(ptr) ((void)0)
#define denormal_fix_double_aggressive(ptr) ((void)0)
#define denormal_fix_float(ptr) ((void)0)
#define denormal_fix_float_aggressive(ptr) ((void)0)

#ifdef __cplusplus
#define denormal_filter(a) (a)
#define denormal_filter_aggressive(a) (a)
#define denormal_fix(ptr) ((void)0)
#define denormal_fix_aggressive(ptr) ((void)0)
#endif

#endif // WDL_DENORMAL_BYPASS



////////////////////
// denormal checker functions, return 0 if denormal/inf/NaN, != 0 otherwise

static int WDL_DENORMAL_INLINE denormal_isnormal_double(double a)
{
  unsigned int hw = WDL_DENORMAL_DOUBLE_HW(&a)&0x7ff00000;
  return hw < 0x7ff00000 ? hw : 0;
}

static int WDL_DENORMAL_INLINE denormal_isnormal_float(float a)
{
  unsigned int hw = WDL_DENORMAL_FLOAT_W(&a)&0x7f800000;
  return hw < 0x7f800000 ? hw : 0;
}

#ifdef __cplusplus

static bool WDL_DENORMAL_INLINE denormal_isnormal(double a)
{
  return !!denormal_isnormal_double(a);
}

static bool WDL_DENORMAL_INLINE denormal_isnormal(float a)
{
  return !!denormal_isnormal_float(a);
}

#endif



////////////////////
// FTZ (flush-to-zero) sets denormal results to 0
// DAZ (denormals-are-zero) treats denormal input as 0

#ifdef WDL_DENORMAL_FTZ

#include <xmmintrin.h> // SSE header, but requires SSE2 (fails with /arch:SSE in MSVC)

static int WDL_DENORMAL_INLINE denormal_get_ftz_mode(void)
{
  return
  #if defined(_MSC_VER) && _MSC_VER < 1700
  _MM_GET_FLUSH_ZERO_MODE(0) // hush warning C4003: not enough actual parameters for macro
  #else
  _MM_GET_FLUSH_ZERO_MODE()
  #endif
  == _MM_FLUSH_ZERO_ON;
}

static void WDL_DENORMAL_INLINE denormal_set_ftz_mode(int mode)
{
  _MM_SET_FLUSH_ZERO_MODE(mode ? _MM_FLUSH_ZERO_ON : _MM_FLUSH_ZERO_OFF);
}

#endif


#ifdef WDL_DENORMAL_DAZ

#include <pmmintrin.h> // requires SSE3

static int WDL_DENORMAL_INLINE denormal_get_daz_mode(void)
{
  return _MM_GET_DENORMALS_ZERO_MODE() == _MM_DENORMALS_ZERO_ON;
}

static void WDL_DENORMAL_INLINE denormal_set_daz_mode(int mode)
{
  _MM_SET_DENORMALS_ZERO_MODE(mode ? _MM_DENORMALS_ZERO_ON : _MM_DENORMALS_ZERO_OFF);
}

#endif


#ifdef __cplusplus

// wrapper class that sets FTZ/DAZ at the beginning of a code block
// automatically restores FTZ/DAZ to their original state at the end of the block

class WDL_DenormalMode
{
public:
  WDL_DenormalMode(bool ftz = true, bool daz = true)
  {
    #if defined(WDL_DENORMAL_FTZ) || defined(WDL_DENORMAL_DAZ)
    int mode = 0;
    #endif

    #ifdef WDL_DENORMAL_FTZ
    {
      const int new_ftz = ftz, old_ftz = denormal_get_ftz_mode();
      mode |= new_ftz | (old_ftz << 1);
      if (new_ftz != old_ftz) denormal_set_ftz_mode(new_ftz);
    }
    #endif

    #ifdef WDL_DENORMAL_DAZ
    {
      const int new_daz = daz, old_daz = denormal_get_daz_mode();
      mode |= (new_daz << 2) | (old_daz << 3);
      if (new_daz != old_daz) denormal_set_daz_mode(new_daz);
    }
    #endif

    #if defined(WDL_DENORMAL_FTZ) || defined(WDL_DENORMAL_DAZ)
    m_mode = mode;
    #endif
  }

  ~WDL_DenormalMode() { Restore(); }

  void Save()
  {
    #if defined(WDL_DENORMAL_FTZ) || defined(WDL_DENORMAL_DAZ)
    int mode = 0;
    #endif

    #ifdef WDL_DENORMAL_FTZ
    {
      const int ftz = denormal_get_ftz_mode();
      mode |= ftz | (ftz << 1);
    }
    #endif

    #ifdef WDL_DENORMAL_DAZ
    {
      const int daz = denormal_get_daz_mode();
      mode |= (daz << 2) | (daz << 3);
    }
    #endif

    #if defined(WDL_DENORMAL_FTZ) || defined(WDL_DENORMAL_DAZ)
    m_mode = mode;
    #endif
  }

  void Restore()
  {
    #ifdef WDL_DENORMAL_FTZ
    {
      const int old_ftz = (m_mode >> 1) & 1;
      if (old_ftz ^ m_mode) denormal_set_ftz_mode(old_ftz);
    }
    #endif

    #ifdef WDL_DENORMAL_DAZ
    {
      const int old_daz = (m_mode >> 3) & 1;
      if (old_daz ^ m_mode) denormal_set_daz_mode(old_daz);
    }
    #endif
  }

  inline bool get_ftz_mode() const
  {
    return
    #ifdef WDL_DENORMAL_FTZ
    !!(m_mode & 1);
    #else
    false;
    #endif
  }

  void set_ftz_mode(bool ftz)
  {
    #ifdef WDL_DENORMAL_FTZ
    const int new_ftz = ftz;
    m_mode ^= (new_ftz ^ m_mode) & 1;
    denormal_set_ftz_mode(new_ftz);
    #endif
  }

  inline bool get_daz_mode() const
  {
    return
    #ifdef WDL_DENORMAL_DAZ
    !!(m_mode & 4);
    #else
    false;
    #endif
  }

  void set_daz_mode(bool daz)
  {
    #ifdef WDL_DENORMAL_DAZ
    const int new_daz = daz;
    m_mode ^= ((new_daz << 2) ^ m_mode) & 4;
    denormal_set_daz_mode(new_daz);
    #endif
  }

private:
  #if defined(WDL_DENORMAL_FTZ) || defined(WDL_DENORMAL_DAZ)
  unsigned int m_mode;
  #endif
};

#endif // WDL_DenormalMode



////////////////////
// this isnt a denormal function but it is similar, so we'll put it here as a bonus

static void WDL_DENORMAL_INLINE GetDoubleMaxAbsValue(double *out, const double *in) // note: the value pointed to by "out" must be >=0.0, __NOT__ <= -0.0
{
  unsigned int hw = WDL_DENORMAL_DOUBLE_HW(in)&0x7fffffff;
  if (hw >= WDL_DENORMAL_DOUBLE_HW(out) && (hw>WDL_DENORMAL_DOUBLE_HW(out) || WDL_DENORMAL_DOUBLE_LW(in) > WDL_DENORMAL_DOUBLE_LW(out)))
  {
    WDL_DENORMAL_DOUBLE_LW_NC(out) = WDL_DENORMAL_DOUBLE_LW(in);
    WDL_DENORMAL_DOUBLE_HW_NC(out) = hw;
  }
}

static void WDL_DENORMAL_INLINE GetFloatMaxAbsValue(float *out, const float *in) // note: the value pointed to by "out" must be >=0.0, __NOT__ <= -0.0
{
  unsigned int hw = WDL_DENORMAL_FLOAT_W(in)&0x7fffffff;
  if (hw > WDL_DENORMAL_FLOAT_W(out)) WDL_DENORMAL_FLOAT_W_NC(out)=hw;
}


#ifdef __cplusplus
static void WDL_DENORMAL_INLINE GetFloatMaxAbsValue(double *out, const double *in) // note: the value pointed to by "out" must be >=0.0, __NOT__ <= -0.0
{
  GetDoubleMaxAbsValue(out,in);
}
#endif



////////////////////
// TN: WDL "Tale" Edition legacy crap, meh. Remove?

#define WDL_NOT_DENORMAL_DOUBLE(ptr) (WDL_DENORMAL_DOUBLE_HW(ptr)&0x7ff00000)
#define WDL_NOT_DENORMAL_FLOAT(ptr) (WDL_DENORMAL_FLOAT_W(ptr)&0x7f800000)
#define WDL_DENORMAL_OR_ZERO_DOUBLE(ptr) (!WDL_NOT_DENORMAL_DOUBLE(ptr))
#define WDL_DENORMAL_OR_ZERO_FLOAT(ptr) (!WDL_NOT_DENORMAL_FLOAT(ptr))
#define WDL_DENORMAL_OR_ZERO_DOUBLE_AGGRESSIVE(ptr) (((WDL_DENORMAL_DOUBLE_HW(ptr)+0x100000)&0x7ff00000) < WDL_DENORMAL_DOUBLE_AGGRESSIVE_CUTOFF)
#define WDL_DENORMAL_OR_ZERO_FLOAT_AGGRESSIVE(ptr) (((WDL_DENORMAL_FLOAT_W(ptr)+0x800000)&0x7f800000) < WDL_DENORMAL_FLOAT_AGGRESSIVE_CUTOFF)

#ifdef __cplusplus
static bool WDL_DENORMAL_INLINE WDL_DENORMAL_OR_ZERO(double *a) { return WDL_DENORMAL_OR_ZERO_DOUBLE(a); }
static bool WDL_DENORMAL_INLINE WDL_DENORMAL_OR_ZERO(float *a) { return WDL_DENORMAL_OR_ZERO_FLOAT(a); }
static bool WDL_DENORMAL_INLINE WDL_DENORMAL_OR_ZERO_AGGRESSIVE(double *a) { return WDL_DENORMAL_OR_ZERO_DOUBLE_AGGRESSIVE(a); }
static bool WDL_DENORMAL_INLINE WDL_DENORMAL_OR_ZERO_AGGRESSIVE(float *a) { return WDL_DENORMAL_OR_ZERO_FLOAT_AGGRESSIVE(a); }
#endif

#endif
