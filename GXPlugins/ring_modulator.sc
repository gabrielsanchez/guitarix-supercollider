FaustRingModulatorMono : UGen
{
  *ar { | in1, dry_wet(0.5), freq(240.0) |
      ^this.multiNew('audio', in1, dry_wet, freq)
  }

  *kr { | in1, dry_wet(0.5), freq(240.0) |
      ^this.multiNew('control', in1, dry_wet, freq)
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

  name { ^"FaustRingModulatorMono" }


  info { ^"Generated with Faust" }
}

