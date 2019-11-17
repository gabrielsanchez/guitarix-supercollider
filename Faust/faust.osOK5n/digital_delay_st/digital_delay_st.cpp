/* ------------------------------------------------------------
name: "Digital Stereo Delay"
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
#include "beat.h"
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
	
	FAUSTFLOAT fVslider0;
	FAUSTFLOAT fHslider0;
	FAUSTFLOAT fVslider1;
	FAUSTFLOAT fHslider1;
	int fSamplingFreq;
	float fConst0;
	float fConst1;
	FAUSTFLOAT fHslider2;
	float fConst2;
	float fConst3;
	float fRec1[2];
	float fRec2[2];
	float fRec3[2];
	float fRec4[2];
	FAUSTFLOAT fHslider3;
	float fConst4;
	FAUSTFLOAT fHslider4;
	FAUSTFLOAT fHslider5;
	FAUSTFLOAT fVslider2;
	float fRec6[3];
	float fRec5[3];
	float fVec0[2];
	float fConst5;
	float fConst6;
	float fConst7;
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
	float fConst24;
	float fConst25;
	float fConst26;
	float fConst27;
	float fConst28;
	float fConst29;
	float fConst30;
	float fConst31;
	float fConst32;
	float fConst33;
	float fConst34;
	float fConst35;
	float fConst36;
	float fConst37;
	float fConst38;
	float fConst39;
	float fConst40;
	float fConst41;
	float fConst42;
	float fConst43;
	float fConst44;
	float fConst45;
	float fConst46;
	float fConst47;
	float fConst48;
	float fConst49;
	float fConst50;
	float fConst51;
	float fRec15[2];
	float fConst52;
	float fConst53;
	float fRec14[3];
	float fConst54;
	float fConst55;
	float fRec13[3];
	float fConst56;
	float fConst57;
	float fConst58;
	float fConst59;
	float fRec12[3];
	float fConst60;
	float fConst61;
	float fConst62;
	float fConst63;
	float fRec11[3];
	float fConst64;
	float fConst65;
	float fConst66;
	float fConst67;
	float fRec10[3];
	float fConst68;
	float fConst69;
	float fConst70;
	float fConst71;
	float fRec9[3];
	float fConst72;
	float fConst73;
	float fConst74;
	float fVec1[2];
	float fRec8[2];
	float fConst75;
	float fConst76;
	float fConst77;
	float fRec7[3];
	float fRec22[2];
	float fRec20[2];
	float fRec18[2];
	float fRec16[2];
	float fRec30[2];
	float fRec28[2];
	float fRec26[2];
	float fRec24[2];
	int IOTA;
	float fVec2[1048576];
	float fRec0[2];
	float fRec34[3];
	float fRec33[3];
	float fVec3[2];
	float fRec43[2];
	float fRec42[3];
	float fRec41[3];
	float fRec40[3];
	float fRec39[3];
	float fRec38[3];
	float fRec37[3];
	float fVec4[2];
	float fRec36[2];
	float fRec35[3];
	float fRec50[2];
	float fRec48[2];
	float fRec46[2];
	float fRec44[2];
	float fRec58[2];
	float fRec56[2];
	float fRec54[2];
	float fRec52[2];
	float fVec5[1048576];
	float fRec32[2];
	
 public:
	
	void metadata(Meta* m) { 
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/version", "0.0");
		m->declare("category", "Echo / Delay");
		m->declare("delays.lib/name", "Faust Delay Library");
		m->declare("delays.lib/version", "0.0");
		m->declare("description", "Digital Delay Stereo Version");
		m->declare("digital_delay.dsp/category", "Echo / Delay");
		m->declare("digital_delay.dsp/description", "Digital Delay");
		m->declare("digital_delay.dsp/id", "dide");
		m->declare("digital_delay.dsp/name", "Digital Delay");
		m->declare("digital_delay.dsp/shortname", "Digi Delay");
		m->declare("filename", "digital_delay_st");
		m->declare("filters.lib/name", "Faust Filters Library");
		m->declare("filters.lib/version", "0.0");
		m->declare("id", "didest");
		m->declare("maths.lib/author", "GRAME");
		m->declare("maths.lib/copyright", "GRAME");
		m->declare("maths.lib/license", "LGPL with exception");
		m->declare("maths.lib/name", "Faust Math Library");
		m->declare("maths.lib/version", "2.1");
		m->declare("name", "Digital Stereo Delay");
		m->declare("shortname", "Digi Delay S");
		m->declare("signals.lib/name", "Faust Signal Routing Library");
		m->declare("signals.lib/version", "0.0");
	}

	virtual int getNumInputs() {
		return 2;
		
	}
	virtual int getNumOutputs() {
		return 2;
		
	}
	virtual int getInputRate(int channel) {
		int rate;
		switch (channel) {
			case 0: {
				rate = 1;
				break;
			}
			case 1: {
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
			case 1: {
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
		fConst1 = (60.0f * fConst0);
		fConst2 = (10.0f / fConst0);
		fConst3 = (0.0f - fConst2);
		fConst4 = (3.14159274f / fConst0);
		fConst5 = std::tan((37699.1133f / fConst0));
		fConst6 = (1.0f / fConst5);
		fConst7 = (((fConst6 + 1.0f) / fConst5) + 1.0f);
		fConst8 = (0.800000012f / fConst7);
		fConst9 = (1.0f / (fConst6 + 1.0f));
		fConst10 = (1.0f - fConst6);
		fConst11 = (std::sin((50265.4844f / fConst0)) * fConst0);
		fConst12 = (3141.59277f / fConst11);
		fConst13 = (25132.7422f / fConst0);
		fConst14 = std::tan(fConst13);
		fConst15 = (1.0f / fConst14);
		fConst16 = (1.0f / (((fConst12 + fConst15) / fConst14) + 1.0f));
		fConst17 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fConst14))));
		fConst18 = (std::sin(fConst13) * fConst0);
		fConst19 = (6268.30127f / fConst18);
		fConst20 = (12566.3711f / fConst0);
		fConst21 = std::tan(fConst20);
		fConst22 = (1.0f / fConst21);
		fConst23 = (1.0f / (((fConst19 + fConst22) / fConst21) + 1.0f));
		fConst24 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fConst21))));
		fConst25 = (std::sin(fConst20) * fConst0);
		fConst26 = (1570.79639f / fConst25);
		fConst27 = std::tan((6283.18555f / fConst0));
		fConst28 = (1.0f / fConst27);
		fConst29 = (1.0f / (((fConst26 + fConst28) / fConst27) + 1.0f));
		fConst30 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fConst27))));
		fConst31 = (std::sin((2356.19458f / fConst0)) * fConst0);
		fConst32 = (392.699097f / fConst31);
		fConst33 = std::tan((1178.09729f / fConst0));
		fConst34 = (1.0f / fConst33);
		fConst35 = (1.0f / (((fConst32 + fConst34) / fConst33) + 1.0f));
		fConst36 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fConst33))));
		fConst37 = (std::sin((1256.63708f / fConst0)) * fConst0);
		fConst38 = (221.880875f / fConst37);
		fConst39 = std::tan((628.318542f / fConst0));
		fConst40 = (1.0f / fConst39);
		fConst41 = (1.0f / (((fConst38 + fConst40) / fConst39) + 1.0f));
		fConst42 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fConst39))));
		fConst43 = std::tan((251.327408f / fConst0));
		fConst44 = (1.0f / fConst43);
		fConst45 = (1.0f / (((fConst44 + 1.0f) / fConst43) + 1.0f));
		fConst46 = mydsp_faustpower2_f(fConst43);
		fConst47 = (1.0f / fConst46);
		fConst48 = (fConst44 + 1.0f);
		fConst49 = (0.0f - (1.0f / (fConst48 * fConst43)));
		fConst50 = (1.0f / fConst48);
		fConst51 = (1.0f - fConst44);
		fConst52 = (((fConst44 + -1.0f) / fConst43) + 1.0f);
		fConst53 = (2.0f * (1.0f - fConst47));
		fConst54 = (0.0f - (2.0f / fConst46));
		fConst55 = (((fConst40 - fConst38) / fConst39) + 1.0f);
		fConst56 = (157.079636f / fConst37);
		fConst57 = (((fConst56 + fConst40) / fConst39) + 1.0f);
		fConst58 = (((fConst40 - fConst56) / fConst39) + 1.0f);
		fConst59 = (((fConst34 - fConst32) / fConst33) + 1.0f);
		fConst60 = (466.723724f / fConst31);
		fConst61 = (((fConst60 + fConst34) / fConst33) + 1.0f);
		fConst62 = (((fConst34 - fConst60) / fConst33) + 1.0f);
		fConst63 = (((fConst28 - fConst26) / fConst27) + 1.0f);
		fConst64 = (2218.80884f / fConst25);
		fConst65 = (((fConst64 + fConst28) / fConst27) + 1.0f);
		fConst66 = (((fConst28 - fConst64) / fConst27) + 1.0f);
		fConst67 = (((fConst22 - fConst19) / fConst21) + 1.0f);
		fConst68 = (3141.59277f / fConst18);
		fConst69 = (((fConst68 + fConst22) / fConst21) + 1.0f);
		fConst70 = (((fConst22 - fConst68) / fConst21) + 1.0f);
		fConst71 = (((fConst15 - fConst12) / fConst14) + 1.0f);
		fConst72 = (3955.03076f / fConst11);
		fConst73 = (((fConst72 + fConst15) / fConst14) + 1.0f);
		fConst74 = (((fConst15 - fConst72) / fConst14) + 1.0f);
		fConst75 = (1.0f / fConst7);
		fConst76 = (((fConst6 + -1.0f) / fConst5) + 1.0f);
		fConst77 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fConst5))));
		
	}
	
	virtual void instanceResetUserInterface() {
		fVslider0 = FAUSTFLOAT(100.0f);
		fHslider0 = FAUSTFLOAT(0.0f);
		fVslider1 = FAUSTFLOAT(50.0f);
		fHslider1 = FAUSTFLOAT(4.0f);
		fHslider2 = FAUSTFLOAT(120.0f);
		fHslider3 = FAUSTFLOAT(0.0f);
		fHslider4 = FAUSTFLOAT(12000.0f);
		fHslider5 = FAUSTFLOAT(120.0f);
		fVslider2 = FAUSTFLOAT(50.0f);
		
	}
	
	virtual void instanceClear() {
		for (int l0 = 0; (l0 < 2); l0 = (l0 + 1)) {
			fRec1[l0] = 0.0f;
			
		}
		for (int l1 = 0; (l1 < 2); l1 = (l1 + 1)) {
			fRec2[l1] = 0.0f;
			
		}
		for (int l2 = 0; (l2 < 2); l2 = (l2 + 1)) {
			fRec3[l2] = 0.0f;
			
		}
		for (int l3 = 0; (l3 < 2); l3 = (l3 + 1)) {
			fRec4[l3] = 0.0f;
			
		}
		for (int l4 = 0; (l4 < 3); l4 = (l4 + 1)) {
			fRec6[l4] = 0.0f;
			
		}
		for (int l5 = 0; (l5 < 3); l5 = (l5 + 1)) {
			fRec5[l5] = 0.0f;
			
		}
		for (int l6 = 0; (l6 < 2); l6 = (l6 + 1)) {
			fVec0[l6] = 0.0f;
			
		}
		for (int l7 = 0; (l7 < 2); l7 = (l7 + 1)) {
			fRec15[l7] = 0.0f;
			
		}
		for (int l8 = 0; (l8 < 3); l8 = (l8 + 1)) {
			fRec14[l8] = 0.0f;
			
		}
		for (int l9 = 0; (l9 < 3); l9 = (l9 + 1)) {
			fRec13[l9] = 0.0f;
			
		}
		for (int l10 = 0; (l10 < 3); l10 = (l10 + 1)) {
			fRec12[l10] = 0.0f;
			
		}
		for (int l11 = 0; (l11 < 3); l11 = (l11 + 1)) {
			fRec11[l11] = 0.0f;
			
		}
		for (int l12 = 0; (l12 < 3); l12 = (l12 + 1)) {
			fRec10[l12] = 0.0f;
			
		}
		for (int l13 = 0; (l13 < 3); l13 = (l13 + 1)) {
			fRec9[l13] = 0.0f;
			
		}
		for (int l14 = 0; (l14 < 2); l14 = (l14 + 1)) {
			fVec1[l14] = 0.0f;
			
		}
		for (int l15 = 0; (l15 < 2); l15 = (l15 + 1)) {
			fRec8[l15] = 0.0f;
			
		}
		for (int l16 = 0; (l16 < 3); l16 = (l16 + 1)) {
			fRec7[l16] = 0.0f;
			
		}
		for (int l17 = 0; (l17 < 2); l17 = (l17 + 1)) {
			fRec22[l17] = 0.0f;
			
		}
		for (int l18 = 0; (l18 < 2); l18 = (l18 + 1)) {
			fRec20[l18] = 0.0f;
			
		}
		for (int l19 = 0; (l19 < 2); l19 = (l19 + 1)) {
			fRec18[l19] = 0.0f;
			
		}
		for (int l20 = 0; (l20 < 2); l20 = (l20 + 1)) {
			fRec16[l20] = 0.0f;
			
		}
		for (int l21 = 0; (l21 < 2); l21 = (l21 + 1)) {
			fRec30[l21] = 0.0f;
			
		}
		for (int l22 = 0; (l22 < 2); l22 = (l22 + 1)) {
			fRec28[l22] = 0.0f;
			
		}
		for (int l23 = 0; (l23 < 2); l23 = (l23 + 1)) {
			fRec26[l23] = 0.0f;
			
		}
		for (int l24 = 0; (l24 < 2); l24 = (l24 + 1)) {
			fRec24[l24] = 0.0f;
			
		}
		IOTA = 0;
		for (int l25 = 0; (l25 < 1048576); l25 = (l25 + 1)) {
			fVec2[l25] = 0.0f;
			
		}
		for (int l26 = 0; (l26 < 2); l26 = (l26 + 1)) {
			fRec0[l26] = 0.0f;
			
		}
		for (int l27 = 0; (l27 < 3); l27 = (l27 + 1)) {
			fRec34[l27] = 0.0f;
			
		}
		for (int l28 = 0; (l28 < 3); l28 = (l28 + 1)) {
			fRec33[l28] = 0.0f;
			
		}
		for (int l29 = 0; (l29 < 2); l29 = (l29 + 1)) {
			fVec3[l29] = 0.0f;
			
		}
		for (int l30 = 0; (l30 < 2); l30 = (l30 + 1)) {
			fRec43[l30] = 0.0f;
			
		}
		for (int l31 = 0; (l31 < 3); l31 = (l31 + 1)) {
			fRec42[l31] = 0.0f;
			
		}
		for (int l32 = 0; (l32 < 3); l32 = (l32 + 1)) {
			fRec41[l32] = 0.0f;
			
		}
		for (int l33 = 0; (l33 < 3); l33 = (l33 + 1)) {
			fRec40[l33] = 0.0f;
			
		}
		for (int l34 = 0; (l34 < 3); l34 = (l34 + 1)) {
			fRec39[l34] = 0.0f;
			
		}
		for (int l35 = 0; (l35 < 3); l35 = (l35 + 1)) {
			fRec38[l35] = 0.0f;
			
		}
		for (int l36 = 0; (l36 < 3); l36 = (l36 + 1)) {
			fRec37[l36] = 0.0f;
			
		}
		for (int l37 = 0; (l37 < 2); l37 = (l37 + 1)) {
			fVec4[l37] = 0.0f;
			
		}
		for (int l38 = 0; (l38 < 2); l38 = (l38 + 1)) {
			fRec36[l38] = 0.0f;
			
		}
		for (int l39 = 0; (l39 < 3); l39 = (l39 + 1)) {
			fRec35[l39] = 0.0f;
			
		}
		for (int l40 = 0; (l40 < 2); l40 = (l40 + 1)) {
			fRec50[l40] = 0.0f;
			
		}
		for (int l41 = 0; (l41 < 2); l41 = (l41 + 1)) {
			fRec48[l41] = 0.0f;
			
		}
		for (int l42 = 0; (l42 < 2); l42 = (l42 + 1)) {
			fRec46[l42] = 0.0f;
			
		}
		for (int l43 = 0; (l43 < 2); l43 = (l43 + 1)) {
			fRec44[l43] = 0.0f;
			
		}
		for (int l44 = 0; (l44 < 2); l44 = (l44 + 1)) {
			fRec58[l44] = 0.0f;
			
		}
		for (int l45 = 0; (l45 < 2); l45 = (l45 + 1)) {
			fRec56[l45] = 0.0f;
			
		}
		for (int l46 = 0; (l46 < 2); l46 = (l46 + 1)) {
			fRec54[l46] = 0.0f;
			
		}
		for (int l47 = 0; (l47 < 2); l47 = (l47 + 1)) {
			fRec52[l47] = 0.0f;
			
		}
		for (int l48 = 0; (l48 < 1048576); l48 = (l48 + 1)) {
			fVec5[l48] = 0.0f;
			
		}
		for (int l49 = 0; (l49 < 2); l49 = (l49 + 1)) {
			fRec32[l49] = 0.0f;
			
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
		ui_interface->openVerticalBox("Digital Stereo Delay");
		ui_interface->declare(&fHslider0, "tooltip", "Freeze the current delay");
		ui_interface->addHorizontalSlider("Freeze", &fHslider0, 0.0f, 0.0f, 1.0f, 1.0f);
		ui_interface->declare(&fHslider2, "name", "BPM");
		ui_interface->declare(&fHslider2, "tooltip", "Delay in Beats per Minute");
		ui_interface->addHorizontalSlider("bpm", &fHslider2, 120.0f, 24.0f, 360.0f, 1.0f);
		ui_interface->declare(&fVslider2, "name", "Feedback");
		ui_interface->declare(&fVslider2, "tooltip", "Percentage of the feedback level in the de.delay loop");
		ui_interface->addVerticalSlider("feedback", &fVslider2, 50.0f, 1.0f, 100.0f, 1.0f);
		ui_interface->declare(&fVslider0, "name", "Gain");
		ui_interface->declare(&fVslider0, "tooltip", "Overall gain of the de.delay line in percent");
		ui_interface->addVerticalSlider("gain", &fVslider0, 100.0f, 0.0f, 120.0f, 1.0f);
		ui_interface->declare(&fHslider5, "name", "Hipass");
		ui_interface->declare(&fHslider5, "tooltip", "Highpass filter frequency in the feddback loop");
		ui_interface->addHorizontalSlider("highpass", &fHslider5, 120.0f, 20.0f, 20000.0f, 1.0f);
		ui_interface->declare(&fHslider4, "name", "Lopass");
		ui_interface->declare(&fHslider4, "tooltip", "Lowpass filter frequency in the feddback loop");
		ui_interface->addHorizontalSlider("howpass", &fHslider4, 12000.0f, 20.0f, 20000.0f, 1.0f);
		ui_interface->declare(&fVslider1, "name", "Level");
		ui_interface->declare(&fVslider1, "tooltip", "Percentage of the de.delay gain level");
		ui_interface->addVerticalSlider("level", &fVslider1, 50.0f, 1.0f, 100.0f, 1.0f);
		ui_interface->declare(&fHslider3, "enum", "plain|presence|tape|tape2");
		ui_interface->declare(&fHslider3, "name", "Mode");
		ui_interface->addHorizontalSlider("mode", &fHslider3, 0.0f, 0.0f, 3.0f, 1.0f);
		ui_interface->declare(&fHslider1, "enum", "Dotted 1/2 note|1/2 note|1/2 note triplets| Dotted 1/4 note|1/4 note|1/4 note triplets|Dotted 1/8 note|1/8 note|1/8 note triplets| Dotted 1/16 note|1/16 note|1/16 note triplets|Dotted 1/32 note|1/32 note|1/32 note triplets| Dotted 1/64 note|1/64 note|1/64 note triplets");
		ui_interface->declare(&fHslider1, "name", "Notes");
		ui_interface->declare(&fHslider1, "tooltip", "Note setting for bpm");
		ui_interface->addHorizontalSlider("notes", &fHslider1, 4.0f, 0.0f, 17.0f, 1.0f);
		ui_interface->closeBox();
		
	}
	
	virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) {
		FAUSTFLOAT* input0 = inputs[0];
		FAUSTFLOAT* input1 = inputs[1];
		FAUSTFLOAT* output0 = outputs[0];
		FAUSTFLOAT* output1 = outputs[1];
		float fSlow0 = (0.00999999978f * float(fVslider0));
		float fSlow1 = float(fHslider0);
		int iSlow2 = int(fSlow1);
		float fSlow3 = (0.00999999978f * float(fVslider1));
		float fSlow4 = std::min<float>(524288.0f, float(B2N(int(float(fHslider1)), float((fConst1 / float(fHslider2))))));
		float fSlow5 = float(fHslider3);
		int iSlow6 = int(std::min<float>(2.0f, std::max<float>(0.0f, (fSlow5 + -2.0f))));
		int iSlow7 = (iSlow6 == 0);
		int iSlow8 = (iSlow6 == 1);
		int iSlow9 = int(std::min<float>(2.0f, std::max<float>(0.0f, (fSlow5 + -1.0f))));
		int iSlow10 = (iSlow9 == 0);
		int iSlow11 = (iSlow9 == 1);
		int iSlow12 = int(std::min<float>(2.0f, std::max<float>(0.0f, fSlow5)));
		int iSlow13 = (iSlow12 == 0);
		int iSlow14 = (iSlow12 == 1);
		float fSlow15 = std::tan((fConst4 * float(fHslider4)));
		float fSlow16 = (1.0f / fSlow15);
		float fSlow17 = (((fSlow16 + 1.41421354f) / fSlow15) + 1.0f);
		float fSlow18 = (1.0f / fSlow17);
		float fSlow19 = std::tan((fConst4 * float(fHslider5)));
		float fSlow20 = (1.0f / fSlow19);
		float fSlow21 = (1.0f / (((fSlow20 + 1.41421354f) / fSlow19) + 1.0f));
		float fSlow22 = mydsp_faustpower2_f(fSlow19);
		float fSlow23 = (1.0f / fSlow22);
		float fSlow24 = (0.00999999978f * float(fVslider2));
		float fSlow25 = (((fSlow20 + -1.41421354f) / fSlow19) + 1.0f);
		float fSlow26 = (2.0f * (1.0f - fSlow23));
		float fSlow27 = (0.0f - (2.0f / fSlow22));
		float fSlow28 = (((fSlow16 + -1.41421354f) / fSlow15) + 1.0f);
		float fSlow29 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fSlow15))));
		float fSlow30 = (fConst44 / fSlow17);
		float fSlow31 = (1.0f - fSlow1);
		for (int i = 0; (i < count); i = (i + 1)) {
			float fTemp0 = ((fRec1[1] != 0.0f)?(((fRec2[1] > 0.0f) & (fRec2[1] < 1.0f))?fRec1[1]:0.0f):(((fRec2[1] == 0.0f) & (fSlow4 != fRec3[1]))?fConst2:(((fRec2[1] == 1.0f) & (fSlow4 != fRec4[1]))?fConst3:0.0f)));
			fRec1[0] = fTemp0;
			fRec2[0] = std::max<float>(0.0f, std::min<float>(1.0f, (fRec2[1] + fTemp0)));
			fRec3[0] = (((fRec2[1] >= 1.0f) & (fRec4[1] != fSlow4))?fSlow4:fRec3[1]);
			fRec4[0] = (((fRec2[1] <= 0.0f) & (fRec3[1] != fSlow4))?fSlow4:fRec4[1]);
			float fTemp1 = (1.0f - fRec2[0]);
			fRec6[0] = ((fSlow24 * fRec0[1]) - (fSlow21 * ((fSlow25 * fRec6[2]) + (fSlow26 * fRec6[1]))));
			fRec5[0] = ((fSlow21 * (((fSlow23 * fRec6[0]) + (fSlow27 * fRec6[1])) + (fSlow23 * fRec6[2]))) - (fSlow18 * ((fSlow28 * fRec5[2]) + (fSlow29 * fRec5[1]))));
			float fTemp2 = (fRec5[2] + (fRec5[0] + (2.0f * fRec5[1])));
			float fTemp3 = (fSlow18 * fTemp2);
			fVec0[0] = fTemp3;
			fRec15[0] = ((fConst49 * fVec0[1]) - (fConst50 * ((fConst51 * fRec15[1]) - (fSlow30 * fTemp2))));
			fRec14[0] = (fRec15[0] - (fConst45 * ((fConst52 * fRec14[2]) + (fConst53 * fRec14[1]))));
			float fTemp4 = (fConst42 * fRec13[1]);
			fRec13[0] = ((fConst45 * (((fConst47 * fRec14[0]) + (fConst54 * fRec14[1])) + (fConst47 * fRec14[2]))) - (fConst41 * ((fConst55 * fRec13[2]) + fTemp4)));
			float fTemp5 = (fConst36 * fRec12[1]);
			fRec12[0] = ((fConst41 * ((fTemp4 + (fConst57 * fRec13[0])) + (fConst58 * fRec13[2]))) - (fConst35 * ((fConst59 * fRec12[2]) + fTemp5)));
			float fTemp6 = (fConst30 * fRec11[1]);
			fRec11[0] = ((fConst35 * ((fTemp5 + (fConst61 * fRec12[0])) + (fConst62 * fRec12[2]))) - (fConst29 * ((fConst63 * fRec11[2]) + fTemp6)));
			float fTemp7 = (fConst24 * fRec10[1]);
			fRec10[0] = ((fConst29 * ((fTemp6 + (fConst65 * fRec11[0])) + (fConst66 * fRec11[2]))) - (fConst23 * ((fConst67 * fRec10[2]) + fTemp7)));
			float fTemp8 = (fConst17 * fRec9[1]);
			fRec9[0] = ((fConst23 * ((fTemp7 + (fConst69 * fRec10[0])) + (fConst70 * fRec10[2]))) - (fConst16 * ((fConst71 * fRec9[2]) + fTemp8)));
			float fTemp9 = ((fTemp8 + (fConst73 * fRec9[0])) + (fConst74 * fRec9[2]));
			fVec1[0] = fTemp9;
			fRec8[0] = (0.0f - (fConst9 * ((fConst10 * fRec8[1]) - (fConst16 * (fTemp9 + fVec1[1])))));
			fRec7[0] = (fRec8[0] - (fConst75 * ((fConst76 * fRec7[2]) + (fConst77 * fRec7[1]))));
			float fTemp10 = (iSlow13?fTemp3:(iSlow14?(fConst8 * (fRec7[2] + (fRec7[0] + (2.0f * fRec7[1])))):fTemp3));
			float fTemp11 = (0.200000003f * fRec18[1]);
			float fTemp12 = (0.100000001f * fRec16[1]);
			float fTemp13 = ((0.400000006f * fRec20[1]) + fTemp12);
			float fTemp14 = ((fTemp11 + (fTemp10 + (0.600000024f * fRec22[1]))) - fTemp13);
			fRec22[0] = fTemp14;
			float fRec23 = (0.0f - (0.600000024f * fTemp14));
			fRec20[0] = (fRec23 + fRec22[1]);
			float fTemp15 = (fTemp10 + fTemp11);
			float fRec21 = (0.400000006f * (fTemp15 - fTemp13));
			fRec18[0] = (fRec21 + fRec20[1]);
			float fRec19 = (0.0f - (0.200000003f * (fTemp15 - fTemp12)));
			fRec16[0] = (fRec19 + fRec18[1]);
			float fRec17 = (0.100000001f * (fTemp10 - fTemp12));
			float fTemp16 = (iSlow10?fTemp10:(iSlow11?(fRec17 + fRec16[1]):fTemp10));
			float fTemp17 = (0.300000012f * fRec28[1]);
			float fTemp18 = (0.5f * fRec24[1]);
			float fTemp19 = ((0.400000006f * fRec26[1]) + fTemp18);
			float fTemp20 = ((fTemp17 + (fTemp16 + (0.200000003f * fRec30[1]))) - fTemp19);
			fRec30[0] = fTemp20;
			float fRec31 = (0.0f - (0.200000003f * fTemp20));
			fRec28[0] = (fRec31 + fRec30[1]);
			float fRec29 = (0.0f - (0.300000012f * ((fTemp16 + fTemp17) - fTemp19)));
			fRec26[0] = (fRec29 + fRec28[1]);
			float fRec27 = (0.400000006f * (fTemp16 - fTemp19));
			fRec24[0] = (fRec27 + fRec26[1]);
			float fRec25 = (0.5f * (fTemp16 - fTemp18));
			float fTemp21 = float(input0[i]);
			float fTemp22 = ((iSlow2?fRec0[1]:(iSlow7?fTemp16:(iSlow8?(fRec25 + fRec24[1]):fTemp16))) + (fSlow31 * fTemp21));
			fVec2[(IOTA & 1048575)] = fTemp22;
			int iTemp23 = int(std::min<float>(524288.0f, std::max<float>(0.0f, fRec3[0])));
			int iTemp24 = int(std::min<float>(524288.0f, std::max<float>(0.0f, fRec4[0])));
			float fTemp25 = ((fTemp1 * fVec2[((IOTA - iTemp23) & 1048575)]) + (fRec2[0] * fVec2[((IOTA - iTemp24) & 1048575)]));
			fRec0[0] = (iSlow2?fTemp25:(fSlow3 * fTemp25));
			output0[i] = FAUSTFLOAT(((fSlow0 * fRec0[0]) + fTemp21));
			fRec34[0] = ((fSlow24 * fRec32[1]) - (fSlow21 * ((fSlow25 * fRec34[2]) + (fSlow26 * fRec34[1]))));
			fRec33[0] = ((fSlow21 * (((fSlow23 * fRec34[0]) + (fSlow27 * fRec34[1])) + (fSlow23 * fRec34[2]))) - (fSlow18 * ((fSlow28 * fRec33[2]) + (fSlow29 * fRec33[1]))));
			float fTemp26 = (fRec33[2] + (fRec33[0] + (2.0f * fRec33[1])));
			float fTemp27 = (fSlow18 * fTemp26);
			fVec3[0] = fTemp27;
			fRec43[0] = ((fConst49 * fVec3[1]) - (fConst50 * ((fConst51 * fRec43[1]) - (fSlow30 * fTemp26))));
			fRec42[0] = (fRec43[0] - (fConst45 * ((fConst52 * fRec42[2]) + (fConst53 * fRec42[1]))));
			float fTemp28 = (fConst42 * fRec41[1]);
			fRec41[0] = ((fConst45 * (((fConst47 * fRec42[0]) + (fConst54 * fRec42[1])) + (fConst47 * fRec42[2]))) - (fConst41 * ((fConst55 * fRec41[2]) + fTemp28)));
			float fTemp29 = (fConst36 * fRec40[1]);
			fRec40[0] = ((fConst41 * ((fTemp28 + (fConst57 * fRec41[0])) + (fConst58 * fRec41[2]))) - (fConst35 * ((fConst59 * fRec40[2]) + fTemp29)));
			float fTemp30 = (fConst30 * fRec39[1]);
			fRec39[0] = ((fConst35 * ((fTemp29 + (fConst61 * fRec40[0])) + (fConst62 * fRec40[2]))) - (fConst29 * ((fConst63 * fRec39[2]) + fTemp30)));
			float fTemp31 = (fConst24 * fRec38[1]);
			fRec38[0] = ((fConst29 * ((fTemp30 + (fConst65 * fRec39[0])) + (fConst66 * fRec39[2]))) - (fConst23 * ((fConst67 * fRec38[2]) + fTemp31)));
			float fTemp32 = (fConst17 * fRec37[1]);
			fRec37[0] = ((fConst23 * ((fTemp31 + (fConst69 * fRec38[0])) + (fConst70 * fRec38[2]))) - (fConst16 * ((fConst71 * fRec37[2]) + fTemp32)));
			float fTemp33 = ((fTemp32 + (fConst73 * fRec37[0])) + (fConst74 * fRec37[2]));
			fVec4[0] = fTemp33;
			fRec36[0] = (0.0f - (fConst9 * ((fConst10 * fRec36[1]) - (fConst16 * (fTemp33 + fVec4[1])))));
			fRec35[0] = (fRec36[0] - (fConst75 * ((fConst76 * fRec35[2]) + (fConst77 * fRec35[1]))));
			float fTemp34 = (iSlow13?fTemp27:(iSlow14?(fConst8 * (fRec35[2] + (fRec35[0] + (2.0f * fRec35[1])))):fTemp27));
			float fTemp35 = (0.200000003f * fRec46[1]);
			float fTemp36 = (0.100000001f * fRec44[1]);
			float fTemp37 = ((0.400000006f * fRec48[1]) + fTemp36);
			float fTemp38 = ((fTemp35 + (fTemp34 + (0.600000024f * fRec50[1]))) - fTemp37);
			fRec50[0] = fTemp38;
			float fRec51 = (0.0f - (0.600000024f * fTemp38));
			fRec48[0] = (fRec51 + fRec50[1]);
			float fTemp39 = (fTemp34 + fTemp35);
			float fRec49 = (0.400000006f * (fTemp39 - fTemp37));
			fRec46[0] = (fRec49 + fRec48[1]);
			float fRec47 = (0.0f - (0.200000003f * (fTemp39 - fTemp36)));
			fRec44[0] = (fRec47 + fRec46[1]);
			float fRec45 = (0.100000001f * (fTemp34 - fTemp36));
			float fTemp40 = (iSlow10?fTemp34:(iSlow11?(fRec45 + fRec44[1]):fTemp34));
			float fTemp41 = (0.300000012f * fRec56[1]);
			float fTemp42 = (0.5f * fRec52[1]);
			float fTemp43 = ((0.400000006f * fRec54[1]) + fTemp42);
			float fTemp44 = ((fTemp41 + (fTemp40 + (0.200000003f * fRec58[1]))) - fTemp43);
			fRec58[0] = fTemp44;
			float fRec59 = (0.0f - (0.200000003f * fTemp44));
			fRec56[0] = (fRec59 + fRec58[1]);
			float fRec57 = (0.0f - (0.300000012f * ((fTemp40 + fTemp41) - fTemp43)));
			fRec54[0] = (fRec57 + fRec56[1]);
			float fRec55 = (0.400000006f * (fTemp40 - fTemp43));
			fRec52[0] = (fRec55 + fRec54[1]);
			float fRec53 = (0.5f * (fTemp40 - fTemp42));
			float fTemp45 = float(input1[i]);
			float fTemp46 = ((iSlow2?fRec32[1]:(iSlow7?fTemp40:(iSlow8?(fRec53 + fRec52[1]):fTemp40))) + (fSlow31 * fTemp45));
			fVec5[(IOTA & 1048575)] = fTemp46;
			float fTemp47 = ((fTemp1 * fVec5[((IOTA - iTemp23) & 1048575)]) + (fRec2[0] * fVec5[((IOTA - iTemp24) & 1048575)]));
			fRec32[0] = (iSlow2?fTemp47:(fSlow3 * fTemp47));
			output1[i] = FAUSTFLOAT(((fSlow0 * fRec32[0]) + fTemp45));
			fRec1[1] = fRec1[0];
			fRec2[1] = fRec2[0];
			fRec3[1] = fRec3[0];
			fRec4[1] = fRec4[0];
			fRec6[2] = fRec6[1];
			fRec6[1] = fRec6[0];
			fRec5[2] = fRec5[1];
			fRec5[1] = fRec5[0];
			fVec0[1] = fVec0[0];
			fRec15[1] = fRec15[0];
			fRec14[2] = fRec14[1];
			fRec14[1] = fRec14[0];
			fRec13[2] = fRec13[1];
			fRec13[1] = fRec13[0];
			fRec12[2] = fRec12[1];
			fRec12[1] = fRec12[0];
			fRec11[2] = fRec11[1];
			fRec11[1] = fRec11[0];
			fRec10[2] = fRec10[1];
			fRec10[1] = fRec10[0];
			fRec9[2] = fRec9[1];
			fRec9[1] = fRec9[0];
			fVec1[1] = fVec1[0];
			fRec8[1] = fRec8[0];
			fRec7[2] = fRec7[1];
			fRec7[1] = fRec7[0];
			fRec22[1] = fRec22[0];
			fRec20[1] = fRec20[0];
			fRec18[1] = fRec18[0];
			fRec16[1] = fRec16[0];
			fRec30[1] = fRec30[0];
			fRec28[1] = fRec28[0];
			fRec26[1] = fRec26[0];
			fRec24[1] = fRec24[0];
			IOTA = (IOTA + 1);
			fRec0[1] = fRec0[0];
			fRec34[2] = fRec34[1];
			fRec34[1] = fRec34[0];
			fRec33[2] = fRec33[1];
			fRec33[1] = fRec33[0];
			fVec3[1] = fVec3[0];
			fRec43[1] = fRec43[0];
			fRec42[2] = fRec42[1];
			fRec42[1] = fRec42[0];
			fRec41[2] = fRec41[1];
			fRec41[1] = fRec41[0];
			fRec40[2] = fRec40[1];
			fRec40[1] = fRec40[0];
			fRec39[2] = fRec39[1];
			fRec39[1] = fRec39[0];
			fRec38[2] = fRec38[1];
			fRec38[1] = fRec38[0];
			fRec37[2] = fRec37[1];
			fRec37[1] = fRec37[0];
			fVec4[1] = fVec4[0];
			fRec36[1] = fRec36[0];
			fRec35[2] = fRec35[1];
			fRec35[1] = fRec35[0];
			fRec50[1] = fRec50[0];
			fRec48[1] = fRec48[0];
			fRec46[1] = fRec46[0];
			fRec44[1] = fRec44[0];
			fRec58[1] = fRec58[0];
			fRec56[1] = fRec56[0];
			fRec54[1] = fRec54[0];
			fRec52[1] = fRec52[0];
			fRec32[1] = fRec32[0];
			
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
