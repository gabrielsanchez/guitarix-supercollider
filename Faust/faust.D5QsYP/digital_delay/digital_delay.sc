FaustDigitalDelay : UGen
{
  *ar { | in1, freeze(0.0), bpm(120.0), feedback(50.0), gain(100.0), highpass(120.0), howpass(12000.0), level(50.0), mode(0.0), notes(4.0) |
      ^this.multiNew('audio', in1, freeze, bpm, feedback, gain, highpass, howpass, level, mode, notes)
  }

  *kr { | in1, freeze(0.0), bpm(120.0), feedback(50.0), gain(100.0), highpass(120.0), howpass(12000.0), level(50.0), mode(0.0), notes(4.0) |
      ^this.multiNew('control', in1, freeze, bpm, feedback, gain, highpass, howpass, level, mode, notes)
  } 

  checkInputs {
    if (rate == 'audio', {
      1.do({|i|
        if (inputs.at(i).rate != 'audio', {
          ^(" input at index " + i + "(" + inputs.at(i) + 
            ") is not audio rate");
        });
      });
    });
    ^this.checkValidInputs
  }

  name { ^"FaustDigitalDelay" }


  info { ^"Generated with Faust" }
}

