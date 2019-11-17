/* ------------------------------------------------------------
name: "pre 12AT7/ master 6V6"
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
	FAUSTFLOAT fVslider1;
	float fRec26[2];
	float fVec0[2];
	float fRec25[2];
	float fRec24[3];
	float fConst24;
	float fConst25;
	float fConst26;
	float fRec27[2];
	float fRec23[3];
	float fConst27;
	float fConst28;
	float fRec22[2];
	float fRec21[2];
	float fRec20[2];
	float fRec19[3];
	float fConst29;
	float fConst30;
	float fConst31;
	float fRec28[2];
	float fRec18[3];
	float fRec17[2];
	float fRec16[2];
	float fRec15[3];
	float fConst32;
	float fConst33;
	float fConst34;
	float fRec29[2];
	float fRec14[3];
	float fRec13[2];
	FAUSTFLOAT fVslider2;
	float fRec30[2];
	float fConst35;
	float fConst36;
	float fRec12[3];
	float fVec1[2];
	float fRec11[2];
	float fRec10[2];
	float fConst37;
	float fConst38;
	float fConst39;
	float fConst40;
	float fRec9[3];
	float fVec2[2];
	float fConst41;
	float fConst42;
	float fConst43;
	float fRec8[2];
	float fConst44;
	float fConst45;
	float fRec7[3];
	float fConst46;
	float fConst47;
	float fConst48;
	float fConst49;
	float fConst50;
	float fConst51;
	float fConst52;
	float fConst53;
	float fConst54;
	float fConst55;
	float fRec6[3];
	float fConst56;
	float fRec34[2];
	float fRec33[3];
	float fConst57;
	float fConst58;
	float fConst59;
	float fRec32[3];
	float fRec31[3];
	float fConst60;
	float fConst61;
	float fConst62;
	float fRec38[2];
	float fRec37[3];
	float fVec3[2];
	float fConst63;
	float fConst64;
	float fRec36[2];
	float fConst65;
	float fRec35[3];
	float fConst66;
	float fConst67;
	float fRec40[2];
	float fRec39[3];
	float fVec4[2];
	float fRec5[2];
	float fVec5[2];
	float fRec4[2];
	FAUSTFLOAT fVslider3;
	float fRec41[2];
	float fVec6[2];
	float fRec3[2];
	float fRec2[3];
	float fRec42[2];
	float fRec1[3];
	float fRec0[2];
	float fRec45[2];
	float fRec44[3];
	float fRec43[2];
	
 public:
	
	void metadata(Meta* m) { 
		m->declare("analyzers.lib/name", "Faust Analyzer Library");
		m->declare("analyzers.lib/version", "0.0");
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/version", "0.0");
		m->declare("filename", "gxamp15");
		m->declare("filters.lib/name", "Faust Filters Library");
		m->declare("filters.lib/version", "0.0");
		m->declare("gxdistortion.dsp/author", "brummer");
		m->declare("gxdistortion.dsp/copyright", "(c)brummer 2008");
		m->declare("gxdistortion.dsp/id", "gxdistortion");
		m->declare("gxdistortion.dsp/license", "BSD");
		m->declare("gxdistortion.dsp/version", "0.01");
		m->declare("id", "pre 12AT7/ master 6V6");
		m->declare("maths.lib/author", "GRAME");
		m->declare("maths.lib/copyright", "GRAME");
		m->declare("maths.lib/license", "LGPL with exception");
		m->declare("maths.lib/name", "Faust Math Library");
		m->declare("maths.lib/version", "2.1");
		m->declare("misceffects.lib/name", "Faust Math Library");
		m->declare("misceffects.lib/version", "2.0");
		m->declare("name", "pre 12AT7/ master 6V6");
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
		fConst24 = (1.0f / std::tan((270.176971f / fConst0)));
		fConst25 = (1.0f / (fConst24 + 1.0f));
		fConst26 = (1.0f - fConst24);
		fConst27 = (0.0f - fConst4);
		fConst28 = ((1.0f - fConst2) / fConst3);
		fConst29 = (1.0f / std::tan((414.690216f / fConst0)));
		fConst30 = (1.0f / (fConst29 + 1.0f));
		fConst31 = (1.0f - fConst29);
		fConst32 = (1.0f / std::tan((609.468994f / fConst0)));
		fConst33 = (1.0f / (fConst32 + 1.0f));
		fConst34 = (1.0f - fConst32);
		fConst35 = (((fConst22 + -1.41421354f) / fConst21) + 1.0f);
		fConst36 = (2.0f * (1.0f - (1.0f / mydsp_faustpower2_f(fConst21))));
		fConst37 = (((fConst14 + -1.0f) / fConst13) + 1.0f);
		fConst38 = mydsp_faustpower2_f(fConst13);
		fConst39 = (1.0f / fConst38);
		fConst40 = (2.0f * (1.0f - fConst39));
		fConst41 = (0.0f - fConst17);
		fConst42 = (1.0f - fConst9);
		fConst43 = (fConst42 / fConst16);
		fConst44 = (((fConst9 + -1.0f) / fConst8) + 1.0f);
		fConst45 = (2.0f * (1.0f - fConst12));
		fConst46 = (0.0f - (2.0f / fConst11));
		fConst47 = std::tan((942.477783f / fConst0));
		fConst48 = (1.0f / fConst47);
		fConst49 = (fConst48 + 1.0f);
		fConst50 = (1.0f / ((fConst49 / fConst47) + 1.0f));
		fConst51 = (1.0f - fConst48);
		fConst52 = (1.0f - (fConst51 / fConst47));
		fConst53 = mydsp_faustpower2_f(fConst47);
		fConst54 = (1.0f / fConst53);
		fConst55 = (2.0f * (1.0f - fConst54));
		fConst56 = (0.0f - (1.0f / (fConst18 * fConst13)));
		fConst57 = (0.0f - (2.0f / fConst38));
		fConst58 = (1.0f / ((fConst16 / fConst8) + 1.0f));
		fConst59 = (1.0f - (fConst42 / fConst8));
		fConst60 = (1.0f / (((fConst48 + 1.0f) / fConst47) + 1.0f));
		fConst61 = (1.0f / (fConst47 * fConst49));
		fConst62 = (1.0f / fConst16);
		fConst63 = (0.0f - fConst61);
		fConst64 = (fConst51 / fConst49);
		fConst65 = (((fConst48 + -1.0f) / fConst47) + 1.0f);
		fConst66 = (0.0f - (2.0f / fConst53));
		fConst67 = (1.0f / fConst49);
		
	}
	
	virtual void instanceResetUserInterface() {
		fVslider0 = FAUSTFLOAT(0.34999999999999998f);
		fVslider1 = FAUSTFLOAT(-6.0f);
		fVslider2 = FAUSTFLOAT(100.0f);
		fVslider3 = FAUSTFLOAT(-6.0f);
		
	}
	
	virtual void instanceClear() {
		for (int l0 = 0; (l0 < 2); l0 = (l0 + 1)) {
			fRec26[l0] = 0.0f;
			
		}
		for (int l1 = 0; (l1 < 2); l1 = (l1 + 1)) {
			fVec0[l1] = 0.0f;
			
		}
		for (int l2 = 0; (l2 < 2); l2 = (l2 + 1)) {
			fRec25[l2] = 0.0f;
			
		}
		for (int l3 = 0; (l3 < 3); l3 = (l3 + 1)) {
			fRec24[l3] = 0.0f;
			
		}
		for (int l4 = 0; (l4 < 2); l4 = (l4 + 1)) {
			fRec27[l4] = 0.0f;
			
		}
		for (int l5 = 0; (l5 < 3); l5 = (l5 + 1)) {
			fRec23[l5] = 0.0f;
			
		}
		for (int l6 = 0; (l6 < 2); l6 = (l6 + 1)) {
			fRec22[l6] = 0.0f;
			
		}
		for (int l7 = 0; (l7 < 2); l7 = (l7 + 1)) {
			fRec21[l7] = 0.0f;
			
		}
		for (int l8 = 0; (l8 < 2); l8 = (l8 + 1)) {
			fRec20[l8] = 0.0f;
			
		}
		for (int l9 = 0; (l9 < 3); l9 = (l9 + 1)) {
			fRec19[l9] = 0.0f;
			
		}
		for (int l10 = 0; (l10 < 2); l10 = (l10 + 1)) {
			fRec28[l10] = 0.0f;
			
		}
		for (int l11 = 0; (l11 < 3); l11 = (l11 + 1)) {
			fRec18[l11] = 0.0f;
			
		}
		for (int l12 = 0; (l12 < 2); l12 = (l12 + 1)) {
			fRec17[l12] = 0.0f;
			
		}
		for (int l13 = 0; (l13 < 2); l13 = (l13 + 1)) {
			fRec16[l13] = 0.0f;
			
		}
		for (int l14 = 0; (l14 < 3); l14 = (l14 + 1)) {
			fRec15[l14] = 0.0f;
			
		}
		for (int l15 = 0; (l15 < 2); l15 = (l15 + 1)) {
			fRec29[l15] = 0.0f;
			
		}
		for (int l16 = 0; (l16 < 3); l16 = (l16 + 1)) {
			fRec14[l16] = 0.0f;
			
		}
		for (int l17 = 0; (l17 < 2); l17 = (l17 + 1)) {
			fRec13[l17] = 0.0f;
			
		}
		for (int l18 = 0; (l18 < 2); l18 = (l18 + 1)) {
			fRec30[l18] = 0.0f;
			
		}
		for (int l19 = 0; (l19 < 3); l19 = (l19 + 1)) {
			fRec12[l19] = 0.0f;
			
		}
		for (int l20 = 0; (l20 < 2); l20 = (l20 + 1)) {
			fVec1[l20] = 0.0f;
			
		}
		for (int l21 = 0; (l21 < 2); l21 = (l21 + 1)) {
			fRec11[l21] = 0.0f;
			
		}
		for (int l22 = 0; (l22 < 2); l22 = (l22 + 1)) {
			fRec10[l22] = 0.0f;
			
		}
		for (int l23 = 0; (l23 < 3); l23 = (l23 + 1)) {
			fRec9[l23] = 0.0f;
			
		}
		for (int l24 = 0; (l24 < 2); l24 = (l24 + 1)) {
			fVec2[l24] = 0.0f;
			
		}
		for (int l25 = 0; (l25 < 2); l25 = (l25 + 1)) {
			fRec8[l25] = 0.0f;
			
		}
		for (int l26 = 0; (l26 < 3); l26 = (l26 + 1)) {
			fRec7[l26] = 0.0f;
			
		}
		for (int l27 = 0; (l27 < 3); l27 = (l27 + 1)) {
			fRec6[l27] = 0.0f;
			
		}
		for (int l28 = 0; (l28 < 2); l28 = (l28 + 1)) {
			fRec34[l28] = 0.0f;
			
		}
		for (int l29 = 0; (l29 < 3); l29 = (l29 + 1)) {
			fRec33[l29] = 0.0f;
			
		}
		for (int l30 = 0; (l30 < 3); l30 = (l30 + 1)) {
			fRec32[l30] = 0.0f;
			
		}
		for (int l31 = 0; (l31 < 3); l31 = (l31 + 1)) {
			fRec31[l31] = 0.0f;
			
		}
		for (int l32 = 0; (l32 < 2); l32 = (l32 + 1)) {
			fRec38[l32] = 0.0f;
			
		}
		for (int l33 = 0; (l33 < 3); l33 = (l33 + 1)) {
			fRec37[l33] = 0.0f;
			
		}
		for (int l34 = 0; (l34 < 2); l34 = (l34 + 1)) {
			fVec3[l34] = 0.0f;
			
		}
		for (int l35 = 0; (l35 < 2); l35 = (l35 + 1)) {
			fRec36[l35] = 0.0f;
			
		}
		for (int l36 = 0; (l36 < 3); l36 = (l36 + 1)) {
			fRec35[l36] = 0.0f;
			
		}
		for (int l37 = 0; (l37 < 2); l37 = (l37 + 1)) {
			fRec40[l37] = 0.0f;
			
		}
		for (int l38 = 0; (l38 < 3); l38 = (l38 + 1)) {
			fRec39[l38] = 0.0f;
			
		}
		for (int l39 = 0; (l39 < 2); l39 = (l39 + 1)) {
			fVec4[l39] = 0.0f;
			
		}
		for (int l40 = 0; (l40 < 2); l40 = (l40 + 1)) {
			fRec5[l40] = 0.0f;
			
		}
		for (int l41 = 0; (l41 < 2); l41 = (l41 + 1)) {
			fVec5[l41] = 0.0f;
			
		}
		for (int l42 = 0; (l42 < 2); l42 = (l42 + 1)) {
			fRec4[l42] = 0.0f;
			
		}
		for (int l43 = 0; (l43 < 2); l43 = (l43 + 1)) {
			fRec41[l43] = 0.0f;
			
		}
		for (int l44 = 0; (l44 < 2); l44 = (l44 + 1)) {
			fVec6[l44] = 0.0f;
			
		}
		for (int l45 = 0; (l45 < 2); l45 = (l45 + 1)) {
			fRec3[l45] = 0.0f;
			
		}
		for (int l46 = 0; (l46 < 3); l46 = (l46 + 1)) {
			fRec2[l46] = 0.0f;
			
		}
		for (int l47 = 0; (l47 < 2); l47 = (l47 + 1)) {
			fRec42[l47] = 0.0f;
			
		}
		for (int l48 = 0; (l48 < 3); l48 = (l48 + 1)) {
			fRec1[l48] = 0.0f;
			
		}
		for (int l49 = 0; (l49 < 2); l49 = (l49 + 1)) {
			fRec0[l49] = 0.0f;
			
		}
		for (int l50 = 0; (l50 < 2); l50 = (l50 + 1)) {
			fRec45[l50] = 0.0f;
			
		}
		for (int l51 = 0; (l51 < 3); l51 = (l51 + 1)) {
			fRec44[l51] = 0.0f;
			
		}
		for (int l52 = 0; (l52 < 2); l52 = (l52 + 1)) {
			fRec43[l52] = 0.0f;
			
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
		ui_interface->openVerticalBox("pre 12AT7/ master 6V6");
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
		float fSlow7 = (fConst60 * std::pow(10.0f, (2.0f * fSlow0)));
		float fSlow8 = (fConst60 * std::pow(10.0f, (0.899999976f * fSlow0)));
		float fSlow9 = (1.25f * fSlow0);
		float fSlow10 = (1.0f - std::max<float>(0.0f, fSlow1));
		float fSlow11 = (0.00100000005f * std::pow(10.0f, (0.0500000007f * float(fVslider3))));
		for (int i = 0; (i < count); i = (i + 1)) {
			fRec26[0] = (fSlow4 + (0.999000013f * fRec26[1]));
			float fTemp0 = (fRec26[0] * float(input0[i]));
			fVec0[0] = fTemp0;
			fRec25[0] = ((0.930284798f * (fTemp0 + fVec0[1])) - (0.860569596f * fRec25[1]));
			fRec24[0] = (fRec25[0] - ((1.84050512f * fRec24[1]) + (0.86129427f * fRec24[2])));
			fRec27[0] = (fConst25 * ((0.0270000007f * (fRec23[1] + fRec23[2])) - (fConst26 * fRec27[1])));
			fRec23[0] = (float(Ftube(int(TUBE_TABLE_12AT7_68k), float((((0.925449848f * (fRec24[0] + fRec24[2])) + (fRec27[0] + (1.8508997f * fRec24[1]))) + -2.61775303f)))) + -153.046188f);
			fRec22[0] = ((0.0250000004f * ((fConst4 * fRec23[0]) + (fConst27 * fRec23[1]))) - (fConst28 * fRec22[1]));
			fRec21[0] = (0.0f - (fConst6 * ((fConst7 * fRec21[1]) - (fRec22[0] + fRec22[1]))));
			fRec20[0] = ((0.930284798f * (fRec21[0] + fRec21[1])) - (0.860569596f * fRec20[1]));
			fRec19[0] = (fRec20[0] - ((1.84050512f * fRec19[1]) + (0.86129427f * fRec19[2])));
			fRec28[0] = (fConst30 * ((0.0149999997f * (fRec18[1] + fRec18[2])) - (fConst31 * fRec28[1])));
			fRec18[0] = (float(Ftube(int(TUBE_TABLE_12AT7_250k), float((((0.925449848f * (fRec19[0] + fRec19[2])) + (fRec28[0] + (1.8508997f * fRec19[1]))) + -1.88733196f)))) + -124.177864f);
			fRec17[0] = ((0.0250000004f * ((fConst4 * fRec18[0]) + (fConst27 * fRec18[1]))) - (fConst28 * fRec17[1]));
			fRec16[0] = ((0.930284798f * (fRec17[0] + fRec17[1])) - (0.860569596f * fRec16[1]));
			fRec15[0] = (fRec16[0] - ((1.84050512f * fRec15[1]) + (0.86129427f * fRec15[2])));
			fRec29[0] = (fConst33 * ((0.00820000004f * (fRec14[1] + fRec14[2])) - (fConst34 * fRec29[1])));
			fRec14[0] = (float(Ftube(int(TUBE_TABLE_12AT7_250k), float((((0.925449848f * (fRec15[0] + fRec15[2])) + (fRec29[0] + (1.8508997f * fRec15[1]))) + -1.25696194f)))) + -96.7119522f);
			fRec13[0] = ((0.0250000004f * ((fConst4 * fRec14[0]) + (fConst27 * fRec14[1]))) - (fConst28 * fRec13[1]));
			fRec30[0] = (fSlow5 + (0.999000013f * fRec30[1]));
			fRec12[0] = ((fRec13[0] * fRec30[0]) - (fConst23 * ((fConst35 * fRec12[2]) + (fConst36 * fRec12[1]))));
			float fTemp1 = (fRec12[2] + (fRec12[0] + (2.0f * fRec12[1])));
			fVec1[0] = fTemp1;
			fRec11[0] = ((fConst23 * ((fConst4 * fTemp1) + (fConst27 * fVec1[1]))) - (fConst28 * fRec11[1]));
			fRec10[0] = (0.0f - (fConst19 * ((fConst20 * fRec10[1]) - (fRec11[0] + fRec11[1]))));
			fRec9[0] = (fRec10[0] - (fConst15 * ((fConst37 * fRec9[2]) + (fConst40 * fRec9[1]))));
			float fTemp2 = (fRec9[2] + (fRec9[0] + (2.0f * fRec9[1])));
			fVec2[0] = fTemp2;
			fRec8[0] = ((fConst15 * ((fConst17 * fTemp2) + (fConst41 * fVec2[1]))) - (fConst43 * fRec8[1]));
			fRec7[0] = (fRec8[0] - (fConst10 * ((fConst44 * fRec7[2]) + (fConst45 * fRec7[1]))));
			float fTemp3 = (fConst55 * fRec6[1]);
			fRec6[0] = ((fConst10 * (((fConst12 * fRec7[0]) + (fConst46 * fRec7[1])) + (fConst12 * fRec7[2]))) - (fConst50 * ((fConst52 * fRec6[2]) + fTemp3)));
			float fTemp4 = std::max<float>(-1.0f, std::min<float>(1.0f, (fSlow3 * (fRec6[2] + (fConst50 * (fTemp3 + (fConst52 * fRec6[0])))))));
			fRec34[0] = ((fConst56 * fRec11[1]) - (fConst19 * ((fConst20 * fRec34[1]) - (fConst14 * fRec11[0]))));
			fRec33[0] = (fRec34[0] - (fConst15 * ((fConst37 * fRec33[2]) + (fConst40 * fRec33[1]))));
			float fTemp5 = (fConst45 * fRec32[1]);
			fRec32[0] = ((fConst15 * (((fConst39 * fRec33[0]) + (fConst57 * fRec33[1])) + (fConst39 * fRec33[2]))) - (fConst58 * ((fConst59 * fRec32[2]) + fTemp5)));
			float fTemp6 = (fConst55 * fRec31[1]);
			fRec31[0] = ((fRec32[2] + (fConst58 * (fTemp5 + (fConst59 * fRec32[0])))) - (fConst50 * ((fConst52 * fRec31[2]) + fTemp6)));
			float fTemp7 = std::max<float>(-1.0f, std::min<float>(1.0f, (fSlow6 * (fRec31[2] + (fConst50 * (fTemp6 + (fConst52 * fRec31[0])))))));
			fRec38[0] = (0.0f - (fConst62 * ((fConst42 * fRec38[1]) - (fConst15 * (fTemp2 + fVec2[1])))));
			fRec37[0] = (fRec38[0] - (fConst10 * ((fConst44 * fRec37[2]) + (fConst45 * fRec37[1]))));
			float fTemp8 = (fRec37[2] + (fRec37[0] + (2.0f * fRec37[1])));
			fVec3[0] = fTemp8;
			fRec36[0] = ((fConst10 * ((fConst61 * fTemp8) + (fConst63 * fVec3[1]))) - (fConst64 * fRec36[1]));
			fRec35[0] = (fRec36[0] - (fConst60 * ((fConst65 * fRec35[2]) + (fConst55 * fRec35[1]))));
			float fTemp9 = std::max<float>(-1.0f, std::min<float>(1.0f, (fSlow7 * (((fConst54 * fRec35[0]) + (fConst66 * fRec35[1])) + (fConst54 * fRec35[2])))));
			fRec40[0] = (0.0f - (fConst67 * ((fConst51 * fRec40[1]) - (fConst10 * (fTemp8 + fVec3[1])))));
			fRec39[0] = (fRec40[0] - (fConst60 * ((fConst65 * fRec39[2]) + (fConst55 * fRec39[1]))));
			float fTemp10 = std::max<float>(-1.0f, std::min<float>(1.0f, (fSlow8 * (fRec39[2] + (fRec39[0] + (2.0f * fRec39[1]))))));
			float fTemp11 = (((1.33352149f * (fTemp4 * (1.0f - (0.333333343f * mydsp_faustpower2_f(fTemp4))))) + (1.58489323f * ((fTemp7 * (1.0f - (0.333333343f * mydsp_faustpower2_f(fTemp7)))) + (fTemp9 * (1.0f - (0.333333343f * mydsp_faustpower2_f(fTemp9))))))) + (1.25894117f * (fTemp10 * (1.0f - (0.333333343f * mydsp_faustpower2_f(fTemp10))))));
			fVec4[0] = fTemp11;
			fRec5[0] = (0.0f - (fConst6 * ((fConst7 * fRec5[1]) - (fTemp11 + fVec4[1]))));
			float fTemp12 = (1.0f - fRec30[0]);
			float fTemp13 = ((fSlow2 * fRec5[0]) + (fRec13[0] * ((fTemp12 * ((fSlow9 * ((4.0f * (1.0f - std::fabs((fRec13[0] * fTemp12)))) + -1.0f)) + 1.0f)) + (fSlow10 * fRec30[0]))));
			fVec5[0] = fTemp13;
			fRec4[0] = (0.0f - (fConst6 * ((fConst7 * fRec4[1]) - (fTemp13 + fVec5[1]))));
			fRec41[0] = (fSlow11 + (0.999000013f * fRec41[1]));
			float fTemp14 = (fRec4[0] * fRec41[0]);
			fVec6[0] = fTemp14;
			fRec3[0] = ((0.930284798f * (fTemp14 + fVec6[1])) - (0.860569596f * fRec3[1]));
			fRec2[0] = (fRec3[0] - ((1.84050512f * fRec2[1]) + (0.86129427f * fRec2[2])));
			float fTemp15 = (0.925449848f * (fRec2[0] + fRec2[2]));
			fRec42[0] = (fConst6 * ((0.00820000004f * (fRec1[1] + fRec1[2])) - (fConst7 * fRec42[1])));
			float fTemp16 = (1.8508997f * fRec2[1]);
			fRec1[0] = (float(Ftube(int(TUBE_TABLE_6V6_250k), float(((fTemp15 + (fRec42[0] + fTemp16)) + -1.13046205f)))) + -112.138779f);
			fRec0[0] = ((0.0250000004f * ((fConst4 * fRec1[0]) + (fConst27 * fRec1[1]))) - (fConst28 * fRec0[1]));
			fRec45[0] = (fConst6 * ((0.00820000004f * (fRec44[1] + fRec44[2])) - (fConst7 * fRec45[1])));
			fRec44[0] = (float(Ftube(int(TUBE_TABLE_6V6_68k), float(((fTemp15 + (fRec45[0] + fTemp16)) + -1.13074005f)))) + -112.104881f);
			fRec43[0] = ((0.0250000004f * ((fConst4 * fRec44[0]) + (fConst27 * fRec44[1]))) - (fConst28 * fRec43[1]));
			output0[i] = FAUSTFLOAT((fRec0[0] + fRec43[0]));
			fRec26[1] = fRec26[0];
			fVec0[1] = fVec0[0];
			fRec25[1] = fRec25[0];
			fRec24[2] = fRec24[1];
			fRec24[1] = fRec24[0];
			fRec27[1] = fRec27[0];
			fRec23[2] = fRec23[1];
			fRec23[1] = fRec23[0];
			fRec22[1] = fRec22[0];
			fRec21[1] = fRec21[0];
			fRec20[1] = fRec20[0];
			fRec19[2] = fRec19[1];
			fRec19[1] = fRec19[0];
			fRec28[1] = fRec28[0];
			fRec18[2] = fRec18[1];
			fRec18[1] = fRec18[0];
			fRec17[1] = fRec17[0];
			fRec16[1] = fRec16[0];
			fRec15[2] = fRec15[1];
			fRec15[1] = fRec15[0];
			fRec29[1] = fRec29[0];
			fRec14[2] = fRec14[1];
			fRec14[1] = fRec14[0];
			fRec13[1] = fRec13[0];
			fRec30[1] = fRec30[0];
			fRec12[2] = fRec12[1];
			fRec12[1] = fRec12[0];
			fVec1[1] = fVec1[0];
			fRec11[1] = fRec11[0];
			fRec10[1] = fRec10[0];
			fRec9[2] = fRec9[1];
			fRec9[1] = fRec9[0];
			fVec2[1] = fVec2[0];
			fRec8[1] = fRec8[0];
			fRec7[2] = fRec7[1];
			fRec7[1] = fRec7[0];
			fRec6[2] = fRec6[1];
			fRec6[1] = fRec6[0];
			fRec34[1] = fRec34[0];
			fRec33[2] = fRec33[1];
			fRec33[1] = fRec33[0];
			fRec32[2] = fRec32[1];
			fRec32[1] = fRec32[0];
			fRec31[2] = fRec31[1];
			fRec31[1] = fRec31[0];
			fRec38[1] = fRec38[0];
			fRec37[2] = fRec37[1];
			fRec37[1] = fRec37[0];
			fVec3[1] = fVec3[0];
			fRec36[1] = fRec36[0];
			fRec35[2] = fRec35[1];
			fRec35[1] = fRec35[0];
			fRec40[1] = fRec40[0];
			fRec39[2] = fRec39[1];
			fRec39[1] = fRec39[0];
			fVec4[1] = fVec4[0];
			fRec5[1] = fRec5[0];
			fVec5[1] = fVec5[0];
			fRec4[1] = fRec4[0];
			fRec41[1] = fRec41[0];
			fVec6[1] = fVec6[0];
			fRec3[1] = fRec3[0];
			fRec2[2] = fRec2[1];
			fRec2[1] = fRec2[0];
			fRec42[1] = fRec42[0];
			fRec1[2] = fRec1[1];
			fRec1[1] = fRec1[0];
			fRec0[1] = fRec0[0];
			fRec45[1] = fRec45[0];
			fRec44[2] = fRec44[1];
			fRec44[1] = fRec44[0];
			fRec43[1] = fRec43[0];
			
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
