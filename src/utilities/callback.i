/* 
 *  Callback support largely unchanged from the contribution of "Carlos" at
 *    http://old.nabble.com/Automatically-generated-callbacks-using-directors-(python)-p19239301.html
 *    
 *  Example.  Consider the following code C++ code.
 * 
 *  @code
 *  
 *  class Foo
 *  {
 *  public:
 *    typedef void (*callback_ptr)(void *, int);
 *    void set_callback(callback_ptr f, void* data)
 *    {
 *      d_f = f;
 *      d_data = data;
 *    }
 *    void call_callback(int v)
 *    {
 *      d_f(d_data, v);
 *    }
 *    callback_ptr d_f;
 *    void*        d_data;
 *  };
 *  
 *  @endcode
 *    
 *  The callback function should have a signature of (int v).  To use the 
 *  callback templates for this example, one would have for a SWIG interface
 *  the following:
 *  
 *  @code 
 *  
 *  %module(directors="1", allprotected="1") foo
 *  %{
 *  #include "foo.hh"
 *  %}
 *  %include callback.i
 *  setCallbackMethod(1, // this is a *unique* identifier
 *                    Foo, 
 *                    set_callback, 
 *                    (void* data, int v), (int v), (v), 1)
 *
 *  %include "foo.hh"
 *  
 *  @endcode
 */
%define setCallbackMethod(num, klass, setter, oldArgs, newArgs, newParams, safe)
__setCallbackSupport__(num, %extend klass {, }, $self->setter, setter, oldArgs, newArgs, newParams, safe)
%enddef

%define setCallbackFunction(num, setter, oldArgs, newArgs, newParams, safe)
__setCallbackSupport__(num, %inline %{, %}, setter, setter, oldArgs, newArgs, newParams, safe)
%enddef

%define __setCallbackSupport__(num, open, close, oldSetter, newSetter, oldArgs, newArgs, newParams, safe)

%feature("director") __Callback_##num##__;

%inline 
%{

class __Callback_##num##__ 
{
public:
  void safeCall##newArgs 
  {
        #if safe == 1
            PyGILState_STATE gstate = PyGILState_Ensure();
            call##newParams;
            PyGILState_Release(gstate);
        #else
            call##newParams;
        #endif 
  }
  virtual void call##newArgs = 0;
  virtual ~__Callback_##num##__() {} 
};

%}

%{
    void __callbackWrapper_##num##__##oldArgs {
        ((__Callback_##num##__*) data)->call##newParams;
    }
%}

%pythoncode 
%{

def __callable2Callback_##num##__(args):
    # len(args) == 2 -> we're in a method, else -> we're in a function
    if len(args) == 2: arg = 1
    else: arg = 0
    if not isinstance(args[arg], __Callback_##num##__):
        from operator import isCallable
        if not isCallable(args[arg]): raise TypeError, "Neither callable nor Callback"
        callable = args[arg]
        class Callback_(__Callback_##num##__):
            def call(self, *args): callable(*args)
        if len(args) == 2: args = (args[0], Callback_(),)
        else: args = (Callback_(),)
    args[arg].__disown__()
    return args

%}

%pythonprepend newSetter %{args = __callable2Callback_##num##__(args)%}

open
    void newSetter(__Callback_##num##__* callback) {
        oldSetter(__callbackWrapper_##num##__, (void*) callback);
    }
close

%enddef