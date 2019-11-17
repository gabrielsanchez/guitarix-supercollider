FaustRedeyePreMono : UGen
{
  *ar { | in1, input(0.0), output(0.0), feedback(-12.0), presence(0.0), resonance(0.0), drive(0.0) |
      ^this.multiNew('audio', in1, input, output, feedback, presence, resonance, drive)
  }

  *kr { | in1, input(0.0), output(0.0), feedback(-12.0), presence(0.0), resonance(0.0), drive(0.0) |
      ^this.multiNew('control', in1, input, output, feedback, presence, resonance, drive)
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

  name { ^"FaustRedeyePreMono" }


  info { ^"Generated with Faust" }
}

