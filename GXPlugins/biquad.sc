FaustBiQuadFilter : UGen
{
  *ar { | in1, freq(1200.0) |
      ^this.multiNew('audio', in1, freq)
  }

  *kr { | in1, freq(1200.0) |
      ^this.multiNew('control', in1, freq)
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

  name { ^"FaustBiQuadFilter" }


  info { ^"Generated with Faust" }
}

