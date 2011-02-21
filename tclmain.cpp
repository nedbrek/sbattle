#include "sim.h"
#include <tcl.h>

class TclData
{
protected:
	Battle theBattle_;
};

int callback(ClientData clientData, Tcl_Interp *interp,
				 int objc, Tcl_Obj *CONST objv[])
{
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
	Tcl_CreateObjCommand(interp, "battle", callback, data, destructor);

	return TCL_OK;
}

