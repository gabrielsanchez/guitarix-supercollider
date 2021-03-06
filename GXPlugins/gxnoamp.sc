Faust : UGen
{
  *ar { | in1, amp2_stage2_gain1(-6.0) |
      ^this.multiNew('audio', in1, amp2_stage2_gain1)
  }

  *kr { | in1, amp2_stage2_gain1(-6.0) |
      ^this.multiNew('control', in1, amp2_stage2_gain1)
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

  name { ^"Faust" }


  info { ^"Generated with Faust" }
}

