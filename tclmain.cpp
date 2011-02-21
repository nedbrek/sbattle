#include "sim.h"
#include <tcl.h>

/// interface to Tcl parameters like an array
class Parms
{
protected:
   Tcl_Interp     *interp_;
   Tcl_Obj *CONST *objv_;
   unsigned        objc_;

public:
   Parms(Tcl_Interp *interp, Tcl_Obj *CONST* objv, unsigned objc):
      interp_(interp), objv_(objv), objc_(objc) {}

   Tcl_Interp* getInterp(void) { return interp_; }

        unsigned getNumArgs(void) const { return objc_-1; }

   int64_t operator[](unsigned i)
   {
      if( i+1 >= objc_ ) return -1;

                Tcl_WideInt ret;
      Tcl_GetWideIntFromObj(interp_, objv_[i+1], &ret);
      return int64_t(ret);
   }

	const char* getStringParm(unsigned i)
   {
      if( i+1 >= objc_ ) return NULL;
      return Tcl_GetStringFromObj(objv_[i+1], NULL);
   }
};

struct TclData
{
	Battle theBattle_;
	unsigned numTrials_;

	TclData(void): numTrials_(0) {}
};

int callback(ClientData clientData, Tcl_Interp *interp,
				 int objc, Tcl_Obj *CONST objv[])
{
	if( objc == 1 ) return TCL_ERROR;

	TclData *data = reinterpret_cast<TclData*>(clientData);

	// build the parms helper
	Parms p(interp, objv, unsigned(objc));

	const char *subcmd = p.getStringParm(0);
	if( strcmp(subcmd, "fight") == 0 )
	{
		a.round(data->theBattle_.fight());
		data->theBattle_.reset();
		data->numTrials_++;
	}
	else if( strcmp(subcmd, "results") == 0 )
	{
		Tcl_Obj *retList = Tcl_NewListObj(0, NULL);
		Tcl_ListObjAppendElement(interp, retList, Tcl_NewIntObj(data->numTrials_));
	   Tcl_ListObjAppendElement(interp, retList, Tcl_NewIntObj(a.bugsWin));
		Tcl_ListObjAppendElement(interp, retList, Tcl_NewIntObj(a.impsWin));
		Tcl_ListObjAppendElement(interp, retList, Tcl_NewIntObj(a.bugsNoHull));
		Tcl_ListObjAppendElement(interp, retList, Tcl_NewIntObj(a.impsNoHull));
		Tcl_ListObjAppendElement(interp, retList, Tcl_NewIntObj(a.bugsNoWeap));
		Tcl_ListObjAppendElement(interp, retList, Tcl_NewIntObj(a.impsNoWeap));
		Tcl_ListObjAppendElement(interp, retList, Tcl_NewIntObj(a.impsShieldRegen));
		Tcl_ListObjAppendElement(interp, retList, Tcl_NewIntObj(a.sumRounds));

		Tcl_SetObjResult(interp, retList);
	}

	return TCL_OK;
}

void destructor(ClientData clientData)
{
	delete reinterpret_cast<TclData*>(clientData);
}

extern "C" int Sbattle_Init(Tcl_Interp *interp);

int Sbattle_Init(Tcl_Interp *interp)
{
	TclData *data = new TclData;
	data->theBattle_.initDefault();

	Tcl_CreateObjCommand(interp, "battle", callback, data, destructor);

	return TCL_OK;
}

