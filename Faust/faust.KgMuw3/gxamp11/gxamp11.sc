Faust12AU7Feedback : UGen
{
  *ar { | in1, gxdistortion_drive(0.35), gxdistortion_wet_dry(100.0), amp2_stage1_pregain(-6.0), amp2_stage2_gain1(-6.0) |
      ^this.multiNew('audio', in1, gxdistortion_drive, gxdistortion_wet_dry, amp2_stage1_pregain, amp2_stage2_gain1)
  }

  *kr { | in1, gxdistortion_drive(0.35), gxdistortion_wet_dry(100.0), amp2_stage1_pregain(-6.0), amp2_stage2_gain1(-6.0) |
      ^this.multiNew('control', in1, gxdistortion_drive, gxdistortion_wet_dry, amp2_stage1_pregain, amp2_stage2_gain1)
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

  name { ^"Faust12AU7Feedback" }


  info { ^"Generated with Faust" }
}

