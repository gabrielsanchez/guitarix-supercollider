FaustFeed : MultiOutUGen
{
  *ar { | in1, amp_feedback(0.0), amp_feedforward(0.0), amp_fuzz(1.0) |
      ^this.multiNew('audio', in1, amp_feedback, amp_feedforward, amp_fuzz)
  }

  *kr { | in1, amp_feedback(0.0), amp_feedforward(0.0), amp_fuzz(1.0) |
      ^this.multiNew('control', in1, amp_feedback, amp_feedforward, amp_fuzz)
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

  init { | ... theInputs |
      inputs = theInputs
      ^this.initOutputs(2, rate)
  }

  name { ^"FaustFeed" }


  info { ^"Generated with Faust" }
}

