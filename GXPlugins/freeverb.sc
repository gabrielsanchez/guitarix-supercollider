FaustFreeverb : UGen
{
  *ar { | in1, roomsize(0.5), damp(0.5), wet_dry(50.0) |
      ^this.multiNew('audio', in1, roomsize, damp, wet_dry)
  }

  *kr { | in1, roomsize(0.5), damp(0.5), wet_dry(50.0) |
      ^this.multiNew('control', in1, roomsize, damp, wet_dry)
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

  name { ^"FaustFreeverb" }


  info { ^"Generated with Faust" }
}

