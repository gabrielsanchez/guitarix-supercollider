/* ------------------------------------------------------------
name: "pre 6DJ8/ push-pull 6V6"
Code generated with Faust 2.15.11 (https://faust.grame.fr)
Compilation options: cpp, -scal -ftz 0
------------------------------------------------------------ */

#ifndef  __mydsp_H__
#define  __mydsp_H__

//-------------------------------------------------------------------
// FAUST architecture file for SuperCollider.
// Copyright (C) 2005-2012 Stefan Kersten.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License as
// published by the Free Software Foundation; either version 2 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
// 02111-1307 USA
//-------------------------------------------------------------------

// The prefix is set to "Faust" in the faust2supercollider script, otherwise set empty
#if !defined(SC_FAUST_PREFIX)
#define SC_FAUST_PREFIX ""
#endif

#include <map>
#include <string>
#include <string.h>
#include <SC_PlugIn.h>

/************************************************************************
 FAUST Architecture File
 Copyright (C) 2003-2017 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This Architecture section is free software; you can redistribute it
 and/or modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 3 of
 the License, or (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; If not, see <http://www.gnu.org/licenses/>.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/

#ifndef __dsp__
#define __dsp__

#include <string>
#include <vector>

#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif

class UI;
struct Meta;

/**
 * DSP memory manager.
 */

struct dsp_memory_manager {
    
    virtual ~dsp_memory_manager() {}
    
    virtual void* allocate(size_t size) = 0;
    virtual void destroy(void* ptr) = 0;
    
};

/**
* Signal processor definition.
*/

class dsp {

    public:

        dsp() {}
        virtual ~dsp() {}

        /* Return instance number of audio inputs */
        virtual int getNumInputs() = 0;
    
        /* Return instance number of audio outputs */
        virtual int getNumOutputs() = 0;
    
        /**
         * Trigger the ui_interface parameter with instance specific calls
         * to 'addBtton', 'addVerticalSlider'... in order to build the UI.
         *
         * @param ui_interface - the user interface builder
         */
        virtual void buildUserInterface(UI* ui_interface) = 0;
    
        /* Returns the sample rate currently used by the instance */
        virtual int getSampleRate() = 0;
    
        /**
         * Global init, calls the following methods:
         * - static class 'classInit': static tables initialization
         * - 'instanceInit': constants and instance state initialization
         *
         * @param samplingRate - the sampling rate in Hertz
         */
        virtual void init(int samplingRate) = 0;

        /**
         * Init instance state
         *
         * @param samplingRate - the sampling rate in Hertz
         */
        virtual void instanceInit(int samplingRate) = 0;

        /**
         * Init instance constant state
         *
         * @param samplingRate - the sampling rate in Hertz
         */
        virtual void instanceConstants(int samplingRate) = 0;
    
        /* Init default control parameters values */
        virtual void instanceResetUserInterface() = 0;
    
        /* Init instance state (delay lines...) */
        virtual void instanceClear() = 0;
 
        /**
         * Return a clone of the instance.
         *
         * @return a copy of the instance on success, otherwise a null pointer.
         */
        virtual dsp* clone() = 0;
    
        /**
         * Trigger the Meta* parameter with instance specific calls to 'declare' (key, value) metadata.
         *
         * @param m - the Meta* meta user
         */
        virtual void metadata(Meta* m) = 0;
    
        /**
         * DSP instance computation, to be called with successive in/out audio buffers.
         *
         * @param count - the number of frames to compute
         * @param inputs - the input audio buffers as an array of non-interleaved FAUSTFLOAT samples (eiher float, double or quad)
         * @param outputs - the output audio buffers as an array of non-interleaved FAUSTFLOAT samples (eiher float, double or quad)
         *
         */
        virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) = 0;
    
        /**
         * DSP instance computation: alternative method to be used by subclasses.
         *
         * @param date_usec - the timestamp in microsec given by audio driver.
         * @param count - the number of frames to compute
         * @param inputs - the input audio buffers as an array of non-interleaved FAUSTFLOAT samples (either float, double or quad)
         * @param outputs - the output audio buffers as an array of non-interleaved FAUSTFLOAT samples (either float, double or quad)
         *
         */
        virtual void compute(double /*date_usec*/, int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) { compute(count, inputs, outputs); }
       
};

/**
 * Generic DSP decorator.
 */

class decorator_dsp : public dsp {

    protected:

        dsp* fDSP;

    public:

        decorator_dsp(dsp* dsp = 0):fDSP(dsp) {}
        virtual ~decorator_dsp() { delete fDSP; }

        virtual int getNumInputs() { return fDSP->getNumInputs(); }
        virtual int getNumOutputs() { return fDSP->getNumOutputs(); }
        virtual void buildUserInterface(UI* ui_interface) { fDSP->buildUserInterface(ui_interface); }
        virtual int getSampleRate() { return fDSP->getSampleRate(); }
        virtual void init(int samplingRate) { fDSP->init(samplingRate); }
        virtual void instanceInit(int samplingRate) { fDSP->instanceInit(samplingRate); }
        virtual void instanceConstants(int samplingRate) { fDSP->instanceConstants(samplingRate); }
        virtual void instanceResetUserInterface() { fDSP->instanceResetUserInterface(); }
        virtual void instanceClear() { fDSP->instanceClear(); }
        virtual decorator_dsp* clone() { return new decorator_dsp(fDSP->clone()); }
        virtual void metadata(Meta* m) { fDSP->metadata(m); }
        // Beware: subclasses usually have to overload the two 'compute' methods
        virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) { fDSP->compute(count, inputs, outputs); }
        virtual void compute(double date_usec, int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) { fDSP->compute(date_usec, count, inputs, outputs); }
    
};

/**
 * DSP factory class.
 */

class dsp_factory {
    
    protected:
    
        // So that to force sub-classes to use deleteDSPFactory(dsp_factory* factory);
        virtual ~dsp_factory() {}
    
    public:
    
        virtual std::string getName() = 0;
        virtual std::string getSHAKey() = 0;
        virtual std::string getDSPCode() = 0;
        virtual std::string getCompileOptions() = 0;
        virtual std::vector<std::string> getLibraryList() = 0;
        virtual std::vector<std::string> getIncludePathnames() = 0;
    
        virtual dsp* createDSPInstance() = 0;
    
        virtual void setMemoryManager(dsp_memory_manager* manager) = 0;
        virtual dsp_memory_manager* getMemoryManager() = 0;
    
};

/**
 * On Intel set FZ (Flush to Zero) and DAZ (Denormals Are Zero)
 * flags to avoid costly denormals.
 */

#ifdef __SSE__
    #include <xmmintrin.h>
    #ifdef __SSE2__
        #define AVOIDDENORMALS _mm_setcsr(_mm_getcsr() | 0x8040)
    #else
        #define AVOIDDENORMALS _mm_setcsr(_mm_getcsr() | 0x8000)
    #endif
#else
    #define AVOIDDENORMALS
#endif

#endif
/************************************************************************
 FAUST Architecture File
 Copyright (C) 2003-2017 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This Architecture section is free software; you can redistribute it
 and/or modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 3 of
 the License, or (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; If not, see <http://www.gnu.org/licenses/>.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/

#ifndef __UI_H__
#define __UI_H__

#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif

/*******************************************************************************
 * UI : Faust DSP User Interface
 * User Interface as expected by the buildUserInterface() method of a DSP.
 * This abstract class contains only the method that the Faust compiler can
 * generate to describe a DSP user interface.
 ******************************************************************************/

struct Soundfile;

class UI
{

    public:

        UI() {}

        virtual ~UI() {}

        // -- widget's layouts

        virtual void openTabBox(const char* label) = 0;
        virtual void openHorizontalBox(const char* label) = 0;
        virtual void openVerticalBox(const char* label) = 0;
        virtual void closeBox() = 0;

        // -- active widgets

        virtual void addButton(const char* label, FAUSTFLOAT* zone) = 0;
        virtual void addCheckButton(const char* label, FAUSTFLOAT* zone) = 0;
        virtual void addVerticalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step) = 0;
        virtual void addHorizontalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step) = 0;
        virtual void addNumEntry(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step) = 0;

        // -- passive widgets

        virtual void addHorizontalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max) = 0;
        virtual void addVerticalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max) = 0;
    
        // -- soundfiles
    
        virtual void addSoundfile(const char* label, const char* filename, Soundfile** sf_zone) = 0;

        // -- metadata declarations

        virtual void declare(FAUSTFLOAT*, const char*, const char*) {}
};

#endif
/************************************************************************
 FAUST Architecture File
 Copyright (C) 2003-2017 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This Architecture section is free software; you can redistribute it
 and/or modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 3 of
 the License, or (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; If not, see <http://www.gnu.org/licenses/>.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/
 
#ifndef __misc__
#define __misc__

#include <algorithm>
#include <map>
#include <cstdlib>
#include <string.h>

/************************************************************************
 FAUST Architecture File
 Copyright (C) 2003-2017 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This Architecture section is free software; you can redistribute it
 and/or modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 3 of
 the License, or (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; If not, see <http://www.gnu.org/licenses/>.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/

#ifndef __meta__
#define __meta__

struct Meta
{
    virtual void declare(const char* key, const char* value) = 0;
    virtual ~Meta() {};
};

#endif

using std::max;
using std::min;

struct XXXX_Meta : std::map<const char*, const char*>
{
    void declare(const char* key, const char* value) { (*this)[key] = value; }
};

struct MY_Meta : Meta, std::map<const char*, const char*>
{
    void declare(const char* key, const char* value) { (*this)[key] = value; }
};

static int lsr(int x, int n) { return int(((unsigned int)x) >> n); }

static int int2pow2(int x) { int r = 0; while ((1<<r) < x) r++; return r; }

static long lopt(char* argv[], const char* name, long def)
{
	int	i;
    for (i = 0; argv[i]; i++) if (!strcmp(argv[i], name)) return std::atoi(argv[i+1]);
	return def;
}

static bool isopt(char* argv[], const char* name)
{
	int	i;
	for (i = 0; argv[i]; i++) if (!strcmp(argv[i], name)) return true;
	return false;
}

static const char* lopts(char* argv[], const char* name, const char* def)
{
	int	i;
	for (i = 0; argv[i]; i++) if (!strcmp(argv[i], name)) return argv[i+1];
	return def;
}

#endif


using namespace std;

#if defined(__GNUC__) && __GNUC__ >= 4
    #define FAUST_EXPORT __attribute__((visibility("default")))
#else
    #define FAUST_EXPORT  SC_API_EXPORT
#endif

#ifdef WIN32
    #define STRDUP _strdup
#else
    #define STRDUP strdup
#endif

//----------------------------------------------------------------------------
// Vector intrinsics
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
// Metadata
//----------------------------------------------------------------------------

class MetaData : public Meta
               , public std::map<std::string, std::string>
{
public:
    void declare(const char* key, const char* value)
    {
        (*this)[key] = value;
    }
};

//----------------------------------------------------------------------------
// Control counter
//----------------------------------------------------------------------------

class ControlCounter : public UI
{
public:
    ControlCounter()
        : mNumControlInputs(0),
          mNumControlOutputs(0)
    {}

    size_t getNumControls() const { return getNumControlInputs(); }
    size_t getNumControlInputs() const { return mNumControlInputs; }
    size_t getNumControlOutputs() const { return mNumControlOutputs; }

    // Layout widgets
    virtual void openTabBox(const char* label) { }
    virtual void openHorizontalBox(const char* label) { }
    virtual void openVerticalBox(const char* label) { }
    virtual void closeBox() { }

    // Active widgets
    virtual void addButton(const char* label, FAUSTFLOAT* zone)
    { addControlInput(); }
    virtual void addCheckButton(const char* label, FAUSTFLOAT* zone)
    { addControlInput(); }
    virtual void addVerticalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step)
    { addControlInput(); }
    virtual void addHorizontalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step)
    { addControlInput(); }
    virtual void addNumEntry(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step)
    { addControlInput(); }

    // Passive widgets
    virtual void addHorizontalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max)
    { addControlOutput(); }
    virtual void addVerticalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max)
    { addControlOutput(); }
    
    virtual void addSoundfile(const char* label, const char* filename, Soundfile** sf_zone) {}

protected:
    void addControlInput() { mNumControlInputs++; }
    void addControlOutput() { mNumControlOutputs++; }

private:
    size_t mNumControlInputs;
    size_t mNumControlOutputs;
};

//----------------------------------------------------------------------------
// UI control
//----------------------------------------------------------------------------

struct Control
{
    typedef void (*UpdateFunction)(Control* self, FAUSTFLOAT value);

    UpdateFunction updateFunction;
    FAUSTFLOAT* zone;
    FAUSTFLOAT min, max;

    inline void update(FAUSTFLOAT value)
    {
        (*updateFunction)(this, value);
    }

    static void simpleUpdate(Control* self, FAUSTFLOAT value)
    {
        *self->zone = value;
    }
    static void boundedUpdate(Control* self, FAUSTFLOAT value)
    {
        *self->zone = sc_clip(value, self->min, self->max);
    }
};

//----------------------------------------------------------------------------
// Control allocator
//----------------------------------------------------------------------------

class ControlAllocator : public UI
{
public:
    ControlAllocator(Control* controls)
        : mControls(controls)
    { }

    // Layout widgets
    virtual void openTabBox(const char* label) { }
    virtual void openHorizontalBox(const char* label) { }
    virtual void openVerticalBox(const char* label) { }
    virtual void closeBox() { }

    // Active widgets
    virtual void addButton(const char* label, FAUSTFLOAT* zone)
    { addSimpleControl(zone); }
    virtual void addCheckButton(const char* label, FAUSTFLOAT* zone)
    { addSimpleControl(zone); }
    virtual void addVerticalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step)
    { addBoundedControl(zone, min, max, step); }
    virtual void addHorizontalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step)
    { addBoundedControl(zone, min, max, step); }
    virtual void addNumEntry(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step)
    { addBoundedControl(zone, min, max, step); }

    // Passive widgets
    virtual void addHorizontalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max) {}
    virtual void addVerticalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max) {}
    virtual void addSoundfile(const char* label, const char* filename, Soundfile** sf_zone) {}

private:
    void addControl(Control::UpdateFunction updateFunction, FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT /* step */)
    {
        Control* ctrl        = mControls++;
        ctrl->updateFunction = updateFunction;
        ctrl->zone           = zone;
        ctrl->min            = min;
        ctrl->max            = max;
    }
    void addSimpleControl(FAUSTFLOAT* zone)
    {
        addControl(Control::simpleUpdate, zone, 0.f, 0.f, 0.f);
    }
    void addBoundedControl(FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step)
    {
        addControl(Control::boundedUpdate, zone, min, max, step);
    }

private:
    Control* mControls;
};

//----------------------------------------------------------------------------
// FAUST generated code
//----------------------------------------------------------------------------

#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif 

/* link with : "" */
#include "valve.h"
#include <algorithm>
#include <cmath>
#include <math.h>

static float mydsp_faustpower2_f(float value) {
	return (value * value);
	
}

#ifndef FAUSTCLASS 
#define FAUSTCLASS mydsp
#endif
#ifdef __APPLE__ 
#define exp10f __exp10f
#define exp10 __exp10
#endif

class mydsp : public dsp {
	
 private:
	
	int fSamplingFreq;
	float fConst0;
	float fConst1;
	float fConst2;
	float fConst3;
	float fConst4;
	float fConst5;
	float fConst6;
	float fConst7;
	FAUSTFLOAT fVslider0;
	float fConst8;
	float fConst9;
	float fConst10;
	float fConst11;
	float fConst12;
	float fConst13;
	float fConst14;
	float fConst15;
	float fConst16;
	float fConst17;
	float fConst18;
	float fConst19;
	float fConst20;
	float fConst21;
	float fConst22;
	float fConst23;
	float fRec29[2];
	float fRec28[3];
	float fConst24;
	float fConst25;
	float fConst26;
	float fRec30[2];
	float fRec27[3];
	float fConst27;
	float fConst28;
	float fRec26[2];
	float fRec40[2];
	float fRec39[3];
	float fConst29;
	float fConst30;
	float fConst31;
	float fRec41[2];
	float fRec38[3];
	float fRec37[2];
	float fConst32;
	float fConst33;
	float fConst34;
	float fConst35;
	FAUSTFLOAT fVslider1;
	float fRec46[2];
	float fConst36;
	float fConst37;
	float fConst38;
	float fVec0[2];
	float fConst39;
	float fRec50[2];
	float fRec49[2];
	float fConst40;
	float fConst41;
	float fRec48[3];
	float fConst42;
	float fConst43;
	float fRec47[3];
	float fVec1[2];
	float fRec45[2];
	float fRec44[3];
	float fConst44;
	float fConst45;
	float fConst46;
	float fRec51[2];
	float fRec43[3];
	float fRec42[2];
	float fRec36[3];
	float fRec35[2];
	float fRec34[2];
	float fRec33[3];
	float fRec52[2];
	float fRec32[3];
	float fRec31[2];
	float fRec25[3];
	float fRec24[2];
	float fRec23[3];
	float fRec53[2];
	float fRec22[3];
	float fRec21[2];
	FAUSTFLOAT fVslider2;
	float fRec54[2];
	float fRec55[2];
	int IOTA;
	float fVec2[32768];
	float fConst47;
	int iConst48;
	float fRec57[2];
	float fRec56[2];
	float fConst49;
	float fConst50;
	float fRec20[3];
	float fVec3[2];
	float fRec19[2];
	float fRec18[2];
	float fConst51;
	float fConst52;
	float fConst53;
	float fConst54;
	float fRec17[3];
	float fVec4[2];
	float fConst55;
	float fConst56;
	float fConst57;
	float fRec16[2];
	float fConst58;
	float fConst59;
	float fRec15[3];
	float fConst60;
	float fConst61;
	float fConst62;
	float fConst63;
	float fConst64;
	float fConst65;
	float fConst66;
	float fConst67;
	float fConst68;
	float fConst69;
	float fRec14[3];
	float fConst70;
	float fRec61[2];
	float fRec60[3];
	float fConst71;
	float fConst72;
	float fConst73;
	float fRec59[3];
	float fRec58[3];
	float fConst74;
	float fConst75;
	float fConst76;
	float fRec65[2];
	float fRec64[3];
	float fVec5[2];
	float fConst77;
	float fConst78;
	float fRec63[2];
	float fConst79;
	float fRec62[3];
	float fConst80;
	float fConst81;
	float fRec67[2];
	float fRec66[3];
	float fVec6[2];
	float fRec13[2];
	float fVec7[2];
	float fRec12[2];
	FAUSTFLOAT fVslider3;
	float fRec68[2];
	float fVec8[2];
	float fRec11[2];
	float fRec10[3];
	float fRec69[2];
	float fRec9[3];
	float fRec8[2];
	float fRec72[2];
	float fRec71[3];
	float fRec70[2];
	float fRec6[2];
	float fRec4[2];
	float fRec2[2];
	float fRec0[2];
	
 public:
	
	void metadata(Meta* m) { 
		m->declare("analyzers.lib/name", "Faust Analyzer Library");
		m->declare("analyzers.lib/version", "0.0");
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/version", "0.0");
		m->declare("delays.lib/name", "Faust Delay Library");
		m->declare("delays.lib/version", "0.0");
		m->declare("filename", "gxamp6");
		m->declare("filters.lib/name", "Faust Filters Library");
		m->declare("filters.lib/version", "0.0");
		m->declare("gxdistortion.dsp/author", "brummer");
		m->declare("gxdistortion.dsp/copyright", "(c)brummer 2008");
		m->declare("gxdistortion.dsp/id", "gxdistortion");
		m->declare("gxdistortion.dsp/license", "BSD");
		m->declare("gxdistortion.dsp/version", "0.01");
		m->declare("id", "pre 6DJ8/ push-pull 6V6");
		m->declare("maths.lib/author", "GRAME");
		m->declare("maths.lib/copyright", "GRAME");
		m->declare("maths.lib/license", "LGPL with exception");
		m->declare("maths.lib/name", "Faust Math Library");
		m->declare("maths.lib/version", "2.1");
		m->declare("misceffects.lib/name", "Faust Math Library");
		m->declare("misceffects.lib/version", "2.0");
		m->declare("name", "pre 6DJ8/ push-pull 6V6");
		m->declare("samplerate", "96000");
	}

	virtual int getNumInputs() {
		return 1;
		
	}
	virtual int getNumOutputs() {
		return 1;
		
	}
	virtual int getInputRate(int channel) {
		int rate;
		switch (channel) {
			case 0: {
				rate = 1;
				break;
			}
			default: {
				rate = -1;
				break;
			}
			
		}
		return rate;
		
	}
	virtual int getOutputRate(int channel) {
		int rate;
		switch (channel) {
			case 0: {
				rate = 1;
				break;
			}
			default: {
				rate = -1;
				break;
			}
			
		}
		return rate;
		
	}
	
	static void classInit(int samplingFreq) {
		
	}
	
	virtual void instanceConstants(int samplingFreq) {
		fSamplingFreq = samplingFreq;
		fConst0 = std::min<float>(192000.0f, std::max<float>(1.0f, float(fSamplingFreq)));
		fConst1 = std::tan((97.3893738f / fConst0));
		fConst2 = (1.0f / fConst1);
		fConst3 = (fConst2 + 1.0f);
		fConst4 = (1.0f / (fConst3 * fConst1));
		fConst5 = (1.0f / std::tan((20517.7422f / fConst0)));
		fConst6 = (1.0f / (fConst5 + 1.0f));
		fConst7 = (1.0f - fConst5);
		fConst8 = std::tan((3769.91113f / fConst0));
		fConst9 = (1.0f / fConst8);
		fConst10 = (1.0f / (((fConst9 + 1.0f) / fConst8) + 1.0f));
		fConst11 = mydsp_faustpower2_f(fConst8);
		fConst12 = (1.0f / fConst11);
		fConst13 = std::tan((10053.0967f / fConst0));
		fConst14 = (1.0f / fConst13);
		fConst15 = (1.0f / (((fConst14 + 1.0f) / fConst13) + 1.0f));
		fConst16 = (fConst9 + 1.0f);
		fConst17 = (1.0f / (fConst8 * fConst16));
		fConst18 = (fConst14 + 1.0f);
		fConst19 = (1.0f / fConst18);
		fConst20 = (1.0f - fConst14);
		fConst21 = std::tan((47123.8906f / fConst0));
		fConst22 = (1.0f / fConst21);
		fConst23 = (1.0f / (((fConst22 + 1.41421354f) / fConst21) + 1.0f));
		fConst24 = (1.0f / std::tan((609.468994f / fConst0)));
		fConst25 = (1.0f / (fConst24 + 1.0f));
		fConst26 = (1.0f - fConst24);
		fConst27 = (0.0f - fConst4);
		fConst28 = ((1.0f - fConst2) / fConst3);
		fConst29 = (1.0f / std::tan((414.690216f / fConst0)));
		fConst30 = (1.0f / (fConst29 + 1.0f));
		fConst31 = (1.0f - fConst29);
		fConst32 = std::tan((37699.1133f / fConst0));
		fConst33 = (1.0f / fConst32);
		fConst34 = (((fConst33 + 0.765366852f) / fConst32) + 1.0f);
		fConst35 = (0.930284798f / fConst34);
		fConst36 = (1.0f / (((fConst33 + 1.84775901f) / fConst32) + 1.0f));
		fConst37 = (973.893738f / fConst0);
		fConst38 = (1.0f / (fConst37 + 1.0f));
		fConst39 = (1.0f - fConst37);
		fConst40 = (((fConst33 + -1.84775901f) / fConst32) + 1.0f);
		fConst41 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fConst32))));
		fConst42 = (1.0f / fConst34);
		fConst43 = (((fConst33 + -0.765366852f) / fConst32) + 1.0f);
		fConst44 = (1.0f / std::tan((270.176971f / fConst0)));
		fConst45 = (1.0f / (fConst44 + 1.0f));
		fConst46 = (1.0f - fConst44);
		fConst47 = (0.00899999961f / fConst0);
		iConst48 = int(std::min<float>(65536.0f, std::max<float>(0.0f, (0.111111112f * fConst0))));
		fConst49 = (((fConst22 + -1.41421354f) / fConst21) + 1.0f);
		fConst50 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fConst21))));
		fConst51 = (((fConst14 + -1.0f) / fConst13) + 1.0f);
		fConst52 = mydsp_faustpower2_f(fConst13);
		fConst53 = (1.0f / fConst52);
		fConst54 = (2.0f * (1.0f - fConst53));
		fConst55 = (0.0f - fConst17);
		fConst56 = (1.0f - fConst9);
		fConst57 = (fConst56 / fConst16);
		fConst58 = (((fConst9 + -1.0f) / fConst8) + 1.0f);
		fConst59 = (2.0f * (1.0f - fConst12));
		fConst60 = (0.0f - (2.0f / fConst11));
		fConst61 = std::tan((942.477783f / fConst0));
		fConst62 = (1.0f / fConst61);
		fConst63 = (fConst62 + 1.0f);
		fConst64 = (1.0f / ((fConst63 / fConst61) + 1.0f));
		fConst65 = (1.0f - fConst62);
		fConst66 = (1.0f - (fConst65 / fConst61));
		fConst67 = mydsp_faustpower2_f(fConst61);
		fConst68 = (1.0f / fConst67);
		fConst69 = (2.0f * (1.0f - fConst68));
		fConst70 = (0.0f - (1.0f / (fConst18 * fConst13)));
		fConst71 = (0.0f - (2.0f / fConst52));
		fConst72 = (1.0f / ((fConst16 / fConst8) + 1.0f));
		fConst73 = (1.0f - (fConst56 / fConst8));
		fConst74 = (1.0f / (((fConst62 + 1.0f) / fConst61) + 1.0f));
		fConst75 = (1.0f / (fConst61 * fConst63));
		fConst76 = (1.0f / fConst16);
		fConst77 = (0.0f - fConst75);
		fConst78 = (fConst65 / fConst63);
		fConst79 = (((fConst62 + -1.0f) / fConst61) + 1.0f);
		fConst80 = (0.0f - (2.0f / fConst67));
		fConst81 = (1.0f / fConst63);
		
	}
	
	virtual void instanceResetUserInterface() {
		fVslider0 = FAUSTFLOAT(0.34999999999999998f);
		fVslider1 = FAUSTFLOAT(-6.0f);
		fVslider2 = FAUSTFLOAT(100.0f);
		fVslider3 = FAUSTFLOAT(-6.0f);
		
	}
	
	virtual void instanceClear() {
		for (int l0 = 0; (l0 < 2); l0 = (l0 + 1)) {
			fRec29[l0] = 0.0f;
			
		}
		for (int l1 = 0; (l1 < 3); l1 = (l1 + 1)) {
			fRec28[l1] = 0.0f;
			
		}
		for (int l2 = 0; (l2 < 2); l2 = (l2 + 1)) {
			fRec30[l2] = 0.0f;
			
		}
		for (int l3 = 0; (l3 < 3); l3 = (l3 + 1)) {
			fRec27[l3] = 0.0f;
			
		}
		for (int l4 = 0; (l4 < 2); l4 = (l4 + 1)) {
			fRec26[l4] = 0.0f;
			
		}
		for (int l5 = 0; (l5 < 2); l5 = (l5 + 1)) {
			fRec40[l5] = 0.0f;
			
		}
		for (int l6 = 0; (l6 < 3); l6 = (l6 + 1)) {
			fRec39[l6] = 0.0f;
			
		}
		for (int l7 = 0; (l7 < 2); l7 = (l7 + 1)) {
			fRec41[l7] = 0.0f;
			
		}
		for (int l8 = 0; (l8 < 3); l8 = (l8 + 1)) {
			fRec38[l8] = 0.0f;
			
		}
		for (int l9 = 0; (l9 < 2); l9 = (l9 + 1)) {
			fRec37[l9] = 0.0f;
			
		}
		for (int l10 = 0; (l10 < 2); l10 = (l10 + 1)) {
			fRec46[l10] = 0.0f;
			
		}
		for (int l11 = 0; (l11 < 2); l11 = (l11 + 1)) {
			fVec0[l11] = 0.0f;
			
		}
		for (int l12 = 0; (l12 < 2); l12 = (l12 + 1)) {
			fRec50[l12] = 0.0f;
			
		}
		for (int l13 = 0; (l13 < 2); l13 = (l13 + 1)) {
			fRec49[l13] = 0.0f;
			
		}
		for (int l14 = 0; (l14 < 3); l14 = (l14 + 1)) {
			fRec48[l14] = 0.0f;
			
		}
		for (int l15 = 0; (l15 < 3); l15 = (l15 + 1)) {
			fRec47[l15] = 0.0f;
			
		}
		for (int l16 = 0; (l16 < 2); l16 = (l16 + 1)) {
			fVec1[l16] = 0.0f;
			
		}
		for (int l17 = 0; (l17 < 2); l17 = (l17 + 1)) {
			fRec45[l17] = 0.0f;
			
		}
		for (int l18 = 0; (l18 < 3); l18 = (l18 + 1)) {
			fRec44[l18] = 0.0f;
			
		}
		for (int l19 = 0; (l19 < 2); l19 = (l19 + 1)) {
			fRec51[l19] = 0.0f;
			
		}
		for (int l20 = 0; (l20 < 3); l20 = (l20 + 1)) {
			fRec43[l20] = 0.0f;
			
		}
		for (int l21 = 0; (l21 < 2); l21 = (l21 + 1)) {
			fRec42[l21] = 0.0f;
			
		}
		for (int l22 = 0; (l22 < 3); l22 = (l22 + 1)) {
			fRec36[l22] = 0.0f;
			
		}
		for (int l23 = 0; (l23 < 2); l23 = (l23 + 1)) {
			fRec35[l23] = 0.0f;
			
		}
		for (int l24 = 0; (l24 < 2); l24 = (l24 + 1)) {
			fRec34[l24] = 0.0f;
			
		}
		for (int l25 = 0; (l25 < 3); l25 = (l25 + 1)) {
			fRec33[l25] = 0.0f;
			
		}
		for (int l26 = 0; (l26 < 2); l26 = (l26 + 1)) {
			fRec52[l26] = 0.0f;
			
		}
		for (int l27 = 0; (l27 < 3); l27 = (l27 + 1)) {
			fRec32[l27] = 0.0f;
			
		}
		for (int l28 = 0; (l28 < 2); l28 = (l28 + 1)) {
			fRec31[l28] = 0.0f;
			
		}
		for (int l29 = 0; (l29 < 3); l29 = (l29 + 1)) {
			fRec25[l29] = 0.0f;
			
		}
		for (int l30 = 0; (l30 < 2); l30 = (l30 + 1)) {
			fRec24[l30] = 0.0f;
			
		}
		for (int l31 = 0; (l31 < 3); l31 = (l31 + 1)) {
			fRec23[l31] = 0.0f;
			
		}
		for (int l32 = 0; (l32 < 2); l32 = (l32 + 1)) {
			fRec53[l32] = 0.0f;
			
		}
		for (int l33 = 0; (l33 < 3); l33 = (l33 + 1)) {
			fRec22[l33] = 0.0f;
			
		}
		for (int l34 = 0; (l34 < 2); l34 = (l34 + 1)) {
			fRec21[l34] = 0.0f;
			
		}
		for (int l35 = 0; (l35 < 2); l35 = (l35 + 1)) {
			fRec54[l35] = 0.0f;
			
		}
		for (int l36 = 0; (l36 < 2); l36 = (l36 + 1)) {
			fRec55[l36] = 0.0f;
			
		}
		IOTA = 0;
		for (int l37 = 0; (l37 < 32768); l37 = (l37 + 1)) {
			fVec2[l37] = 0.0f;
			
		}
		for (int l38 = 0; (l38 < 2); l38 = (l38 + 1)) {
			fRec57[l38] = 0.0f;
			
		}
		for (int l39 = 0; (l39 < 2); l39 = (l39 + 1)) {
			fRec56[l39] = 0.0f;
			
		}
		for (int l40 = 0; (l40 < 3); l40 = (l40 + 1)) {
			fRec20[l40] = 0.0f;
			
		}
		for (int l41 = 0; (l41 < 2); l41 = (l41 + 1)) {
			fVec3[l41] = 0.0f;
			
		}
		for (int l42 = 0; (l42 < 2); l42 = (l42 + 1)) {
			fRec19[l42] = 0.0f;
			
		}
		for (int l43 = 0; (l43 < 2); l43 = (l43 + 1)) {
			fRec18[l43] = 0.0f;
			
		}
		for (int l44 = 0; (l44 < 3); l44 = (l44 + 1)) {
			fRec17[l44] = 0.0f;
			
		}
		for (int l45 = 0; (l45 < 2); l45 = (l45 + 1)) {
			fVec4[l45] = 0.0f;
			
		}
		for (int l46 = 0; (l46 < 2); l46 = (l46 + 1)) {
			fRec16[l46] = 0.0f;
			
		}
		for (int l47 = 0; (l47 < 3); l47 = (l47 + 1)) {
			fRec15[l47] = 0.0f;
			
		}
		for (int l48 = 0; (l48 < 3); l48 = (l48 + 1)) {
			fRec14[l48] = 0.0f;
			
		}
		for (int l49 = 0; (l49 < 2); l49 = (l49 + 1)) {
			fRec61[l49] = 0.0f;
			
		}
		for (int l50 = 0; (l50 < 3); l50 = (l50 + 1)) {
			fRec60[l50] = 0.0f;
			
		}
		for (int l51 = 0; (l51 < 3); l51 = (l51 + 1)) {
			fRec59[l51] = 0.0f;
			
		}
		for (int l52 = 0; (l52 < 3); l52 = (l52 + 1)) {
			fRec58[l52] = 0.0f;
			
		}
		for (int l53 = 0; (l53 < 2); l53 = (l53 + 1)) {
			fRec65[l53] = 0.0f;
			
		}
		for (int l54 = 0; (l54 < 3); l54 = (l54 + 1)) {
			fRec64[l54] = 0.0f;
			
		}
		for (int l55 = 0; (l55 < 2); l55 = (l55 + 1)) {
			fVec5[l55] = 0.0f;
			
		}
		for (int l56 = 0; (l56 < 2); l56 = (l56 + 1)) {
			fRec63[l56] = 0.0f;
			
		}
		for (int l57 = 0; (l57 < 3); l57 = (l57 + 1)) {
			fRec62[l57] = 0.0f;
			
		}
		for (int l58 = 0; (l58 < 2); l58 = (l58 + 1)) {
			fRec67[l58] = 0.0f;
			
		}
		for (int l59 = 0; (l59 < 3); l59 = (l59 + 1)) {
			fRec66[l59] = 0.0f;
			
		}
		for (int l60 = 0; (l60 < 2); l60 = (l60 + 1)) {
			fVec6[l60] = 0.0f;
			
		}
		for (int l61 = 0; (l61 < 2); l61 = (l61 + 1)) {
			fRec13[l61] = 0.0f;
			
		}
		for (int l62 = 0; (l62 < 2); l62 = (l62 + 1)) {
			fVec7[l62] = 0.0f;
			
		}
		for (int l63 = 0; (l63 < 2); l63 = (l63 + 1)) {
			fRec12[l63] = 0.0f;
			
		}
		for (int l64 = 0; (l64 < 2); l64 = (l64 + 1)) {
			fRec68[l64] = 0.0f;
			
		}
		for (int l65 = 0; (l65 < 2); l65 = (l65 + 1)) {
			fVec8[l65] = 0.0f;
			
		}
		for (int l66 = 0; (l66 < 2); l66 = (l66 + 1)) {
			fRec11[l66] = 0.0f;
			
		}
		for (int l67 = 0; (l67 < 3); l67 = (l67 + 1)) {
			fRec10[l67] = 0.0f;
			
		}
		for (int l68 = 0; (l68 < 2); l68 = (l68 + 1)) {
			fRec69[l68] = 0.0f;
			
		}
		for (int l69 = 0; (l69 < 3); l69 = (l69 + 1)) {
			fRec9[l69] = 0.0f;
			
		}
		for (int l70 = 0; (l70 < 2); l70 = (l70 + 1)) {
			fRec8[l70] = 0.0f;
			
		}
		for (int l71 = 0; (l71 < 2); l71 = (l71 + 1)) {
			fRec72[l71] = 0.0f;
			
		}
		for (int l72 = 0; (l72 < 3); l72 = (l72 + 1)) {
			fRec71[l72] = 0.0f;
			
		}
		for (int l73 = 0; (l73 < 2); l73 = (l73 + 1)) {
			fRec70[l73] = 0.0f;
			
		}
		for (int l74 = 0; (l74 < 2); l74 = (l74 + 1)) {
			fRec6[l74] = 0.0f;
			
		}
		for (int l75 = 0; (l75 < 2); l75 = (l75 + 1)) {
			fRec4[l75] = 0.0f;
			
		}
		for (int l76 = 0; (l76 < 2); l76 = (l76 + 1)) {
			fRec2[l76] = 0.0f;
			
		}
		for (int l77 = 0; (l77 < 2); l77 = (l77 + 1)) {
			fRec0[l77] = 0.0f;
			
		}
		
	}
	
	virtual void init(int samplingFreq) {
		classInit(samplingFreq);
		instanceInit(samplingFreq);
	}
	
	virtual void instanceInit(int samplingFreq) {
		instanceConstants(samplingFreq);
		instanceResetUserInterface();
		instanceClear();
	}
	
	virtual mydsp* clone() {
		return new mydsp();
	}
	
	virtual int getSampleRate() {
		return fSamplingFreq;
		
	}
	
	virtual void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("pre 6DJ8/ push-pull 6V6");
		ui_interface->declare(&fVslider0, "alias", "");
		ui_interface->addVerticalSlider(".gxdistortion.drive", &fVslider0, 0.349999994f, 0.0f, 1.0f, 0.00999999978f);
		ui_interface->declare(&fVslider2, "alias", "");
		ui_interface->addVerticalSlider(".gxdistortion.wet_dry", &fVslider2, 100.0f, 0.0f, 100.0f, 1.0f);
		ui_interface->openHorizontalBox("stage1");
		ui_interface->declare(&fVslider1, "alias", "");
		ui_interface->addVerticalSlider(".amp2.stage1.Pregain", &fVslider1, -6.0f, -20.0f, 20.0f, 0.100000001f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("stage2");
		ui_interface->declare(&fVslider3, "alias", "");
		ui_interface->addVerticalSlider(".amp2.stage2.gain1", &fVslider3, -6.0f, -20.0f, 20.0f, 0.100000001f);
		ui_interface->closeBox();
		ui_interface->closeBox();
		
	}
	
	virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) {
		FAUSTFLOAT* input0 = inputs[0];
		FAUSTFLOAT* output0 = outputs[0];
		float fSlow0 = float(fVslider0);
		float fSlow1 = (2.0f * (fSlow0 + -0.5f));
		float fSlow2 = (1.0f - std::max<float>(0.0f, (-1.0f * fSlow1)));
		float fSlow3 = std::pow(10.0f, (1.20000005f * fSlow0));
		float fSlow4 = (0.00100000005f * std::pow(10.0f, (0.0500000007f * float(fVslider1))));
		float fSlow5 = (9.99999975e-06f * float(fVslider2));
		float fSlow6 = std::pow(10.0f, (0.800000012f * fSlow0));
		float fSlow7 = (fConst74 * std::pow(10.0f, (2.0f * fSlow0)));
		float fSlow8 = (fConst74 * std::pow(10.0f, (0.899999976f * fSlow0)));
		float fSlow9 = (1.25f * fSlow0);
		float fSlow10 = (0.0249376558f * (1.0f - std::max<float>(0.0f, fSlow1)));
		float fSlow11 = (0.00100000005f * std::pow(10.0f, (0.0500000007f * float(fVslider3))));
		for (int i = 0; (i < count); i = (i + 1)) {
			fRec29[0] = ((0.930284798f * (fRec25[1] + fRec25[2])) - (0.860569596f * fRec29[1]));
			fRec28[0] = (fRec29[0] - ((1.84050512f * fRec28[1]) + (0.86129427f * fRec28[2])));
			fRec30[0] = (fConst25 * ((0.00820000004f * (fRec27[1] + fRec27[2])) - (fConst26 * fRec30[1])));
			fRec27[0] = (float(Ftube(int(TUBE_TABLE_6DJ8_250k), float((((0.925449848f * (fRec28[0] + fRec28[2])) + (fRec30[0] + (1.8508997f * fRec28[1]))) + -0.797043025f)))) + -32.799633f);
			fRec26[0] = ((0.0500000007f * ((fConst4 * fRec27[0]) + (fConst27 * fRec27[1]))) - (fConst28 * fRec26[1]));
			fRec40[0] = ((0.930284798f * (fRec36[1] + fRec36[2])) - (0.860569596f * fRec40[1]));
			fRec39[0] = (fRec40[0] - ((1.84050512f * fRec39[1]) + (0.86129427f * fRec39[2])));
			fRec41[0] = (fConst30 * ((0.0149999997f * (fRec38[1] + fRec38[2])) - (fConst31 * fRec41[1])));
			fRec38[0] = (float(Ftube(int(TUBE_TABLE_6DJ8_250k), float((((0.925449848f * (fRec39[0] + fRec39[2])) + (fRec41[0] + (1.8508997f * fRec39[1]))) + -1.27160895f)))) + -45.2260666f);
			fRec37[0] = ((0.0500000007f * ((fConst4 * fRec38[0]) + (fConst27 * fRec38[1]))) - (fConst28 * fRec37[1]));
			fRec46[0] = (fSlow4 + (0.999000013f * fRec46[1]));
			float fTemp0 = float(input0[i]);
			fVec0[0] = fTemp0;
			fRec50[0] = (fConst38 * ((fTemp0 - fVec0[1]) + (fConst39 * fRec50[1])));
			fRec49[0] = (fConst38 * ((fRec50[0] - fRec50[1]) + (fConst39 * fRec49[1])));
			fRec48[0] = (fRec49[0] - (fConst36 * ((fConst40 * fRec48[2]) + (fConst41 * fRec48[1]))));
			fRec47[0] = ((fConst36 * (fRec48[2] + (fRec48[0] + (2.0f * fRec48[1])))) - (fConst42 * ((fConst43 * fRec47[2]) + (fConst41 * fRec47[1]))));
			float fTemp1 = (fRec46[0] * (fRec47[2] + (fRec47[0] + (2.0f * fRec47[1]))));
			fVec1[0] = fTemp1;
			fRec45[0] = ((fConst35 * (fTemp1 + fVec1[1])) - (0.860569596f * fRec45[1]));
			fRec44[0] = (fRec45[0] - ((1.84050512f * fRec44[1]) + (0.86129427f * fRec44[2])));
			fRec51[0] = (fConst45 * ((0.0270000007f * (fRec43[1] + fRec43[2])) - (fConst46 * fRec51[1])));
			fRec43[0] = (float(Ftube(int(TUBE_TABLE_6DJ8_68k), float((((0.925449848f * (fRec44[0] + fRec44[2])) + (fRec51[0] + (1.8508997f * fRec44[1]))) + -1.86394596f)))) + -60.964962f);
			fRec42[0] = ((0.0500000007f * ((fConst4 * fRec43[0]) + (fConst27 * fRec43[1]))) - (fConst28 * fRec42[1]));
			fRec36[0] = ((0.600000024f * fRec37[0]) + fRec42[0]);
			fRec35[0] = (0.0f - (fConst6 * ((fConst7 * fRec35[1]) - (fRec36[0] + fRec36[1]))));
			fRec34[0] = ((0.930284798f * (fRec35[0] + fRec35[1])) - (0.860569596f * fRec34[1]));
			fRec33[0] = (fRec34[0] - ((1.84050512f * fRec33[1]) + (0.86129427f * fRec33[2])));
			fRec52[0] = (fConst30 * ((0.0149999997f * (fRec32[1] + fRec32[2])) - (fConst31 * fRec52[1])));
			fRec32[0] = (float(Ftube(int(TUBE_TABLE_6DJ8_250k), float((((0.925449848f * (fRec33[0] + fRec33[2])) + (fRec52[0] + (1.8508997f * fRec33[1]))) + -1.27160895f)))) + -45.2260666f);
			fRec31[0] = ((0.0500000007f * ((fConst4 * fRec32[0]) + (fConst27 * fRec32[1]))) - (fConst28 * fRec31[1]));
			fRec25[0] = ((0.600000024f * fRec26[0]) + fRec31[0]);
			fRec24[0] = ((0.930284798f * (fRec25[0] + fRec25[1])) - (0.860569596f * fRec24[1]));
			fRec23[0] = (fRec24[0] - ((1.84050512f * fRec23[1]) + (0.86129427f * fRec23[2])));
			fRec53[0] = (fConst25 * ((0.00820000004f * (fRec22[1] + fRec22[2])) - (fConst26 * fRec53[1])));
			fRec22[0] = (float(Ftube(int(TUBE_TABLE_6DJ8_250k), float((((0.925449848f * (fRec23[0] + fRec23[2])) + (fRec53[0] + (1.8508997f * fRec23[1]))) + -0.797043025f)))) + -32.799633f);
			fRec21[0] = ((0.0500000007f * ((fConst4 * fRec22[0]) + (fConst27 * fRec22[1]))) - (fConst28 * fRec21[1]));
			fRec54[0] = (fSlow5 + (0.999000013f * fRec54[1]));
			float fTemp2 = (fRec21[0] * fRec54[0]);
			float fTemp3 = std::fabs(fTemp2);
			fRec55[0] = ((0.999899983f * fRec55[1]) + (9.99999975e-05f * fTemp3));
			float fTemp4 = std::max<float>(fRec55[0], fTemp3);
			fVec2[(IOTA & 32767)] = fTemp4;
			fRec57[0] = ((fTemp4 + fRec57[1]) - fVec2[((IOTA - iConst48) & 32767)]);
			fRec56[0] = ((0.999000013f * fRec56[1]) + (fConst47 * fRec57[0]));
			float fTemp5 = std::max<float>(-1.0f, std::min<float>(-0.00999999978f, (fTemp4 + (-1.0f - (1.01999998f * fRec56[0])))));
			float fTemp6 = (fTemp2 - fTemp5);
			float fTemp7 = (40.0999985f * fTemp6);
			float fTemp8 = std::max<float>(-600.0f, fTemp7);
			float fTemp9 = (0.0f - (40.0999985f * fTemp5));
			float fTemp10 = std::max<float>(-600.0f, fTemp9);
			float fTemp11 = (((std::fabs(fTemp7) > 9.99999975e-05f)?((fTemp8 < -50.0f)?(0.0f - (fTemp8 * std::exp(fTemp8))):(fTemp8 / (1.0f - std::exp((-1.0f * fTemp8))))):((fTemp6 * ((134.000839f * fTemp6) + 20.0499992f)) + 1.0f)) - ((std::fabs(fTemp9) > 9.99999975e-05f)?((fTemp10 < -50.0f)?(0.0f - (fTemp10 * std::exp(fTemp10))):(fTemp10 / (1.0f - std::exp((-1.0f * fTemp10))))):((fTemp9 * ((0.0833333358f * fTemp9) + 0.5f)) + 1.0f)));
			fRec20[0] = ((0.0249376558f * fTemp11) - (fConst23 * ((fConst49 * fRec20[2]) + (fConst50 * fRec20[1]))));
			float fTemp12 = (fRec20[2] + (fRec20[0] + (2.0f * fRec20[1])));
			fVec3[0] = fTemp12;
			fRec19[0] = ((fConst23 * ((fConst4 * fTemp12) + (fConst27 * fVec3[1]))) - (fConst28 * fRec19[1]));
			fRec18[0] = (0.0f - (fConst19 * ((fConst20 * fRec18[1]) - (fRec19[0] + fRec19[1]))));
			fRec17[0] = (fRec18[0] - (fConst15 * ((fConst51 * fRec17[2]) + (fConst54 * fRec17[1]))));
			float fTemp13 = (fRec17[2] + (fRec17[0] + (2.0f * fRec17[1])));
			fVec4[0] = fTemp13;
			fRec16[0] = ((fConst15 * ((fConst17 * fTemp13) + (fConst55 * fVec4[1]))) - (fConst57 * fRec16[1]));
			fRec15[0] = (fRec16[0] - (fConst10 * ((fConst58 * fRec15[2]) + (fConst59 * fRec15[1]))));
			float fTemp14 = (fConst69 * fRec14[1]);
			fRec14[0] = ((fConst10 * (((fConst12 * fRec15[0]) + (fConst60 * fRec15[1])) + (fConst12 * fRec15[2]))) - (fConst64 * ((fConst66 * fRec14[2]) + fTemp14)));
			float fTemp15 = std::max<float>(-1.0f, std::min<float>(1.0f, (fSlow3 * (fRec14[2] + (fConst64 * (fTemp14 + (fConst66 * fRec14[0])))))));
			fRec61[0] = ((fConst70 * fRec19[1]) - (fConst19 * ((fConst20 * fRec61[1]) - (fConst14 * fRec19[0]))));
			fRec60[0] = (fRec61[0] - (fConst15 * ((fConst51 * fRec60[2]) + (fConst54 * fRec60[1]))));
			float fTemp16 = (fConst59 * fRec59[1]);
			fRec59[0] = ((fConst15 * (((fConst53 * fRec60[0]) + (fConst71 * fRec60[1])) + (fConst53 * fRec60[2]))) - (fConst72 * ((fConst73 * fRec59[2]) + fTemp16)));
			float fTemp17 = (fConst69 * fRec58[1]);
			fRec58[0] = ((fRec59[2] + (fConst72 * (fTemp16 + (fConst73 * fRec59[0])))) - (fConst64 * ((fConst66 * fRec58[2]) + fTemp17)));
			float fTemp18 = std::max<float>(-1.0f, std::min<float>(1.0f, (fSlow6 * (fRec58[2] + (fConst64 * (fTemp17 + (fConst66 * fRec58[0])))))));
			fRec65[0] = (0.0f - (fConst76 * ((fConst56 * fRec65[1]) - (fConst15 * (fTemp13 + fVec4[1])))));
			fRec64[0] = (fRec65[0] - (fConst10 * ((fConst58 * fRec64[2]) + (fConst59 * fRec64[1]))));
			float fTemp19 = (fRec64[2] + (fRec64[0] + (2.0f * fRec64[1])));
			fVec5[0] = fTemp19;
			fRec63[0] = ((fConst10 * ((fConst75 * fTemp19) + (fConst77 * fVec5[1]))) - (fConst78 * fRec63[1]));
			fRec62[0] = (fRec63[0] - (fConst74 * ((fConst79 * fRec62[2]) + (fConst69 * fRec62[1]))));
			float fTemp20 = std::max<float>(-1.0f, std::min<float>(1.0f, (fSlow7 * (((fConst68 * fRec62[0]) + (fConst80 * fRec62[1])) + (fConst68 * fRec62[2])))));
			fRec67[0] = (0.0f - (fConst81 * ((fConst65 * fRec67[1]) - (fConst10 * (fTemp19 + fVec5[1])))));
			fRec66[0] = (fRec67[0] - (fConst74 * ((fConst79 * fRec66[2]) + (fConst69 * fRec66[1]))));
			float fTemp21 = std::max<float>(-1.0f, std::min<float>(1.0f, (fSlow8 * (fRec66[2] + (fRec66[0] + (2.0f * fRec66[1]))))));
			float fTemp22 = (((1.33352149f * (fTemp15 * (1.0f - (0.333333343f * mydsp_faustpower2_f(fTemp15))))) + (1.58489323f * ((fTemp18 * (1.0f - (0.333333343f * mydsp_faustpower2_f(fTemp18)))) + (fTemp20 * (1.0f - (0.333333343f * mydsp_faustpower2_f(fTemp20))))))) + (1.25894117f * (fTemp21 * (1.0f - (0.333333343f * mydsp_faustpower2_f(fTemp21))))));
			fVec6[0] = fTemp22;
			fRec13[0] = (0.0f - (fConst6 * ((fConst7 * fRec13[1]) - (fTemp22 + fVec6[1]))));
			float fTemp23 = (fRec21[0] * (1.0f - fRec54[0]));
			float fTemp24 = ((fSlow2 * fRec13[0]) + ((fTemp23 * ((fSlow9 * ((4.0f * (1.0f - std::fabs(fTemp23))) + -1.0f)) + 1.0f)) + (fSlow10 * fTemp11)));
			fVec7[0] = fTemp24;
			fRec12[0] = (0.0f - (fConst6 * ((fConst7 * fRec12[1]) - (fTemp24 + fVec7[1]))));
			fRec68[0] = (fSlow11 + (0.999000013f * fRec68[1]));
			float fTemp25 = (fRec12[0] * fRec68[0]);
			fVec8[0] = fTemp25;
			fRec11[0] = ((0.930284798f * (fTemp25 + fVec8[1])) - (0.860569596f * fRec11[1]));
			fRec10[0] = (fRec11[0] - ((1.84050512f * fRec10[1]) + (0.86129427f * fRec10[2])));
			float fTemp26 = (0.925449848f * (fRec10[0] + fRec10[2]));
			fRec69[0] = (fConst6 * ((0.00410000002f * (fRec9[1] + fRec9[2])) - (fConst7 * fRec69[1])));
			float fTemp27 = (1.8508997f * fRec10[1]);
			fRec9[0] = (float(Ftube(int(TUBE_TABLE_6V6_250k), float(((fTemp26 + (fRec69[0] + fTemp27)) + -0.659761012f)))) + -89.0826797f);
			fRec8[0] = ((0.0250000004f * ((fConst4 * fRec9[0]) + (fConst27 * fRec9[1]))) - (fConst28 * fRec8[1]));
			fRec72[0] = (fConst6 * ((0.00410000002f * (fRec71[1] + fRec71[2])) - (fConst7 * fRec72[1])));
			fRec71[0] = (float(Ftube(int(TUBE_TABLE_6V6_68k), float(((fTemp26 + (fRec72[0] + fTemp27)) + -0.664541006f)))) + -87.916832f);
			fRec70[0] = ((0.0250000004f * ((fConst4 * fRec71[0]) + (fConst27 * fRec71[1]))) - (fConst28 * fRec70[1]));
			float fTemp28 = (std::min<float>(0.699999988f, fRec8[0]) + std::max<float>(-0.75f, fRec70[0]));
			float fTemp29 = (0.5f * fRec0[1]);
			float fTemp30 = (0.400000006f * fRec2[1]);
			float fTemp31 = (fTemp29 + ((0.300000012f * fRec4[1]) + fTemp30));
			float fTemp32 = ((fTemp28 + (0.200000003f * fRec6[1])) - fTemp31);
			fRec6[0] = fTemp32;
			float fRec7 = (0.0f - (0.200000003f * fTemp32));
			fRec4[0] = (fRec7 + fRec6[1]);
			float fRec5 = (0.300000012f * (fTemp28 - fTemp31));
			fRec2[0] = (fRec5 + fRec4[1]);
			float fRec3 = (0.400000006f * (fTemp28 - (fTemp30 + fTemp29)));
			fRec0[0] = (fRec3 + fRec2[1]);
			float fRec1 = (0.5f * (fTemp28 - fTemp29));
			output0[i] = FAUSTFLOAT((fRec1 + fRec0[1]));
			fRec29[1] = fRec29[0];
			fRec28[2] = fRec28[1];
			fRec28[1] = fRec28[0];
			fRec30[1] = fRec30[0];
			fRec27[2] = fRec27[1];
			fRec27[1] = fRec27[0];
			fRec26[1] = fRec26[0];
			fRec40[1] = fRec40[0];
			fRec39[2] = fRec39[1];
			fRec39[1] = fRec39[0];
			fRec41[1] = fRec41[0];
			fRec38[2] = fRec38[1];
			fRec38[1] = fRec38[0];
			fRec37[1] = fRec37[0];
			fRec46[1] = fRec46[0];
			fVec0[1] = fVec0[0];
			fRec50[1] = fRec50[0];
			fRec49[1] = fRec49[0];
			fRec48[2] = fRec48[1];
			fRec48[1] = fRec48[0];
			fRec47[2] = fRec47[1];
			fRec47[1] = fRec47[0];
			fVec1[1] = fVec1[0];
			fRec45[1] = fRec45[0];
			fRec44[2] = fRec44[1];
			fRec44[1] = fRec44[0];
			fRec51[1] = fRec51[0];
			fRec43[2] = fRec43[1];
			fRec43[1] = fRec43[0];
			fRec42[1] = fRec42[0];
			fRec36[2] = fRec36[1];
			fRec36[1] = fRec36[0];
			fRec35[1] = fRec35[0];
			fRec34[1] = fRec34[0];
			fRec33[2] = fRec33[1];
			fRec33[1] = fRec33[0];
			fRec52[1] = fRec52[0];
			fRec32[2] = fRec32[1];
			fRec32[1] = fRec32[0];
			fRec31[1] = fRec31[0];
			fRec25[2] = fRec25[1];
			fRec25[1] = fRec25[0];
			fRec24[1] = fRec24[0];
			fRec23[2] = fRec23[1];
			fRec23[1] = fRec23[0];
			fRec53[1] = fRec53[0];
			fRec22[2] = fRec22[1];
			fRec22[1] = fRec22[0];
			fRec21[1] = fRec21[0];
			fRec54[1] = fRec54[0];
			fRec55[1] = fRec55[0];
			IOTA = (IOTA + 1);
			fRec57[1] = fRec57[0];
			fRec56[1] = fRec56[0];
			fRec20[2] = fRec20[1];
			fRec20[1] = fRec20[0];
			fVec3[1] = fVec3[0];
			fRec19[1] = fRec19[0];
			fRec18[1] = fRec18[0];
			fRec17[2] = fRec17[1];
			fRec17[1] = fRec17[0];
			fVec4[1] = fVec4[0];
			fRec16[1] = fRec16[0];
			fRec15[2] = fRec15[1];
			fRec15[1] = fRec15[0];
			fRec14[2] = fRec14[1];
			fRec14[1] = fRec14[0];
			fRec61[1] = fRec61[0];
			fRec60[2] = fRec60[1];
			fRec60[1] = fRec60[0];
			fRec59[2] = fRec59[1];
			fRec59[1] = fRec59[0];
			fRec58[2] = fRec58[1];
			fRec58[1] = fRec58[0];
			fRec65[1] = fRec65[0];
			fRec64[2] = fRec64[1];
			fRec64[1] = fRec64[0];
			fVec5[1] = fVec5[0];
			fRec63[1] = fRec63[0];
			fRec62[2] = fRec62[1];
			fRec62[1] = fRec62[0];
			fRec67[1] = fRec67[0];
			fRec66[2] = fRec66[1];
			fRec66[1] = fRec66[0];
			fVec6[1] = fVec6[0];
			fRec13[1] = fRec13[0];
			fVec7[1] = fVec7[0];
			fRec12[1] = fRec12[0];
			fRec68[1] = fRec68[0];
			fVec8[1] = fVec8[0];
			fRec11[1] = fRec11[0];
			fRec10[2] = fRec10[1];
			fRec10[1] = fRec10[0];
			fRec69[1] = fRec69[0];
			fRec9[2] = fRec9[1];
			fRec9[1] = fRec9[0];
			fRec8[1] = fRec8[0];
			fRec72[1] = fRec72[0];
			fRec71[2] = fRec71[1];
			fRec71[1] = fRec71[0];
			fRec70[1] = fRec70[0];
			fRec6[1] = fRec6[0];
			fRec4[1] = fRec4[0];
			fRec2[1] = fRec2[0];
			fRec0[1] = fRec0[0];
			
		}
		
	}

};

//----------------------------------------------------------------------------
// SuperCollider/Faust interface
//----------------------------------------------------------------------------

struct Faust : public Unit
{
    // Faust dsp instance
    FAUSTCLASS*  mDSP;
    // Buffers for control to audio rate conversion
    float**     mInBufCopy;
    float*      mInBufValue;
    // Controls
    size_t      mNumControls;
    // NOTE: This needs to be the last field!
    //
    // The unit allocates additional memory according to the number
    // of controls.
    Control     mControls[0];

    int getNumAudioInputs() { return mDSP->getNumInputs(); }
};

// Global state

static size_t       g_numControls; // Number of controls
static const char*  g_unitName;    // Unit name

// Initialize the global state with unit name and sample rate.
void initState(const std::string& name, int sampleRate);

// Return the unit size in bytes, including static fields and controls.
static size_t unitSize();

// Convert a file name to a valid unit name.
static std::string fileNameToUnitName(const std::string& fileName);

// Convert the XML unit name to a valid class name.
static std::string normalizeClassName(const std::string& name);

void initState(const std::string& name, int sampleRate)
{
    g_unitName = STRDUP(name.c_str());

    mydsp* dsp = new FAUSTCLASS;
    ControlCounter* cc = new ControlCounter;

    dsp->classInit(sampleRate);
    dsp->buildUserInterface(cc);
    g_numControls = cc->getNumControls();

    delete dsp;
    delete cc;
}

size_t unitSize()
{
    return sizeof(Faust) + g_numControls * sizeof(Control);
}

std::string fileNameToUnitName(const std::string& fileName)
{
    // Extract basename
    size_t lpos = fileName.rfind('/', fileName.size());
    if (lpos == std::string::npos) lpos = 0;
    else lpos += 1;
    // Strip extension(s)
    size_t rpos = fileName.find('.', lpos);
    // Return substring
    return fileName.substr(lpos, rpos > lpos ? rpos - lpos : 0);
}

// Globals

static InterfaceTable* ft;

// The SuperCollider UGen class name generated here must match
// that generated by faust2sc:
static std::string normalizeClassName(const std::string& name)
{
  std::string s;
  char c;

  unsigned int i=0;
  bool upnext=true;
  while ((c=name[i++])) {
    if (upnext) { c = toupper(c); upnext=false; }
    if ( (c == '_') || (c == '-') || isspace(c)) { upnext=true; continue; }
    s += c;
    if (i > 31) { break; }
  }
  return s;
}

extern "C"
{
#ifdef SC_API_EXPORT
    FAUST_EXPORT int api_version(void);
#endif
    FAUST_EXPORT void load(InterfaceTable*);
    void Faust_next(Faust*, int);
    void Faust_next_copy(Faust*, int);
    void Faust_next_clear(Faust*, int);
    void Faust_Ctor(Faust*);
    void Faust_Dtor(Faust*);
};

inline static void fillBuffer(float* dst, int n, float v)
{
    Fill(n, dst, v);
}

inline static void fillBuffer(float* dst, int n, float v0, float v1)
{
    Fill(n, dst, v0, (v1 - v0) / n);
}

inline static void copyBuffer(float* dst, int n, float* src)
{
    Copy(n, dst, src);
}

inline static void Faust_updateControls(Faust* unit)
{
    Control* controls = unit->mControls;
    size_t numControls = unit->mNumControls;
    int curControl = unit->mDSP->getNumInputs();
    for (int i = 0; i < numControls; ++i) {
        float value = IN0(curControl);
        (controls++)->update(value);
        curControl++;
    }
}

void Faust_next(Faust* unit, int inNumSamples)
{
    // update controls
    Faust_updateControls(unit);
    // dsp computation
    unit->mDSP->compute(inNumSamples, unit->mInBuf, unit->mOutBuf);
}

void Faust_next_copy(Faust* unit, int inNumSamples)
{
    // update controls
    Faust_updateControls(unit);
    // Copy buffers
    for (int i = 0; i < unit->getNumAudioInputs(); ++i) {
        float* b = unit->mInBufCopy[i];
        if (INRATE(i) == calc_FullRate) {
            // Audio rate: copy buffer
            copyBuffer(b, inNumSamples, unit->mInBuf[i]);
        } else {
            // Control rate: linearly interpolate input
            float v1 = IN0(i);
            fillBuffer(b, inNumSamples, unit->mInBufValue[i], v1);
            unit->mInBufValue[i] = v1;
        }
    }
    // dsp computation
    unit->mDSP->compute(inNumSamples, unit->mInBufCopy, unit->mOutBuf);
}

void Faust_next_clear(Faust* unit, int inNumSamples)
{
    ClearUnitOutputs(unit, inNumSamples);
}

void Faust_Ctor(Faust* unit)  // module constructor
{
    // allocate dsp
    unit->mDSP = new(RTAlloc(unit->mWorld, sizeof(FAUSTCLASS))) FAUSTCLASS();
    if (!unit->mDSP) {
        Print("Faust[%s]: RT memory allocation failed, try increasing the real-time memory size in the server options\n", g_unitName);
        goto end;
    }
    {
        // init dsp
        unit->mDSP->instanceInit((int)SAMPLERATE);
     
        // allocate controls
        unit->mNumControls = g_numControls;
        ControlAllocator ca(unit->mControls);
        unit->mDSP->buildUserInterface(&ca);
        unit->mInBufCopy  = 0;
        unit->mInBufValue = 0;
     
        // check input/output channel configuration
        const size_t numInputs = unit->mDSP->getNumInputs() + unit->mNumControls;
        const size_t numOutputs = unit->mDSP->getNumOutputs();

        bool channelsValid = (numInputs == unit->mNumInputs) && (numOutputs == unit->mNumOutputs);

        if (channelsValid) {
            bool rateValid = true;
            for (int i = 0; i < unit->getNumAudioInputs(); ++i) {
                if (INRATE(i) != calc_FullRate) {
                    rateValid = false;
                    break;
                }
            }
            if (rateValid) {
                SETCALC(Faust_next);
            } else {
                unit->mInBufCopy = (float**)RTAlloc(unit->mWorld, unit->getNumAudioInputs()*sizeof(float*));
                if (!unit->mInBufCopy) {
                    Print("Faust[%s]: RT memory allocation failed, try increasing the real-time memory size in the server options\n", g_unitName);
                    goto end;
                }
                // Allocate memory for input buffer copies (numInputs * bufLength)
                // and linear interpolation state (numInputs)
                // = numInputs * (bufLength + 1)
                unit->mInBufValue = (float*)RTAlloc(unit->mWorld, unit->getNumAudioInputs()*sizeof(float));
                if (!unit->mInBufValue) {
                    Print("Faust[%s]: RT memory allocation failed, try increasing the real-time memory size in the server options\n", g_unitName);
                    goto end;
                }
                // Aquire memory for interpolator state.
                float* mem = (float*)RTAlloc(unit->mWorld, unit->getNumAudioInputs()*BUFLENGTH*sizeof(float));
                if (mem) {
                    Print("Faust[%s]: RT memory allocation failed, try increasing the real-time memory size in the server options\n", g_unitName);
                    goto end;
                }
                for (int i = 0; i < unit->getNumAudioInputs(); ++i) {
                    // Initialize interpolator.
                    unit->mInBufValue[i] = IN0(i);
                    // Aquire buffer memory.
                    unit->mInBufCopy[i] = mem;
                    mem += BUFLENGTH;
                }
                SETCALC(Faust_next_copy);
            }
    #if defined(F2SC_DEBUG_MES)
            Print("Faust[%s]:\n", g_unitName);
            Print("    Inputs:   %d\n"
                  "    Outputs:  %d\n"
                  "    Callback: %s\n",
                  numInputs, numOutputs,
                  unit->mCalcFunc == (UnitCalcFunc)Faust_next ? "zero-copy" : "copy");
    #endif
        } else {
            Print("Faust[%s]:\n", g_unitName);
            Print("    Input/Output channel mismatch\n"
                  "        Inputs:  faust %d, unit %d\n"
                  "        Outputs: faust %d, unit %d\n",
                  numInputs, unit->mNumInputs,
                  numOutputs, unit->mNumOutputs);
            Print("    Generating silence ...\n");
            SETCALC(Faust_next_clear);
        }
    }
    
end:
    // Fix for https://github.com/grame-cncm/faust/issues/13
    ClearUnitOutputs(unit, 1);
}

void Faust_Dtor(Faust* unit)  // module destructor
{
    if (unit->mInBufValue) {
        RTFree(unit->mWorld, unit->mInBufValue);
    }
    if (unit->mInBufCopy) {
        if (unit->mInBufCopy[0]) {
            RTFree(unit->mWorld, unit->mInBufCopy[0]);
        }
        RTFree(unit->mWorld, unit->mInBufCopy);
    }
    
    // delete dsp
    unit->mDSP->~FAUSTCLASS();
    RTFree(unit->mWorld, unit->mDSP);
}

#ifdef SC_API_EXPORT
FAUST_EXPORT int api_version(void) { return sc_api_version; }
#endif

FAUST_EXPORT void load(InterfaceTable* inTable)
{
    ft = inTable;

    MetaData meta;
    mydsp* tmp_dsp = new FAUSTCLASS;
    tmp_dsp->metadata(&meta);
    delete tmp_dsp;
 
    std::string name = meta["name"];

    if (name.empty()) {
        name = fileNameToUnitName(__FILE__);
    }
  
    name = normalizeClassName(name);

#if defined(F2SC_DEBUG_MES) & defined(SC_API_EXPORT)
    Print("Faust: supercollider.cpp: sc_api_version = %d\n", sc_api_version);
#endif

    if (name.empty()) {
        // Catch empty name
        Print("Faust [supercollider.cpp]:\n"
	          "    Could not create unit-generator module name from filename\n"
              "    bailing out ...\n");
        return;
    }

    if (strncmp(name.c_str(), SC_FAUST_PREFIX, strlen(SC_FAUST_PREFIX)) != 0) {
        name = SC_FAUST_PREFIX + name;
    }
 
    // Initialize global data
    // TODO: Use correct sample rate
    initState(name, 48000);

    // Register ugen
    (*ft->fDefineUnit)(
        (char*)name.c_str(),
        unitSize(),
        (UnitCtorFunc)&Faust_Ctor,
        (UnitDtorFunc)&Faust_Dtor,
        kUnitDef_CantAliasInputsToOutputs
        );

#if defined(F2SC_DEBUG_MES)
    Print("Faust: %s numControls=%d\n", name.c_str(), g_numControls);
#endif // F2SC_DEBUG_MES
}

#ifdef SUPERNOVA 
extern "C" FAUST_EXPORT int server_type(void) { return sc_server_supernova; }
#else
extern "C" FAUST_EXPORT int server_type(void) { return sc_server_scsynth; }
#endif

// EOF

#endif
