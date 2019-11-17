FaustRedeyePreStereo : MultiOutUGen
{
  *ar { | in1, in2, input(0.0), output(0.0), feedback(-12.0), presence(0.0), resonance(0.0), drive(0.0) |
      ^this.multiNew('audio', in1, in2, input, output, feedback, presence, resonance, drive)
  }

  *kr { | in1, in2, input(0.0), output(0.0), feedback(-12.0), presence(0.0), resonance(0.0), drive(0.0) |
      ^this.multiNew('control', in1, in2, input, output, feedback, presence, resonance, drive)
  } 

  checkInputs {
    if (rate == 'audio', {
      2.do({|i|
        if (inputs.at(i).rate != 'audio', {
          ^(" input at index " + i + "(" + inputs.at(i) + 
            ") is not audio rate");
        });
      });
    });
    ^this.checkValidInputs
  }

  init { | ... theInputs |
      inputs = theInputs
      ^this.initOutputs(2, rate)
  }

  name { ^"FaustRedeyePreStereo" }


  info { ^"Generated with Faust" }
}

