# Exercises for State Machine and QA

## Exercise1: Implement an oven controller

We covered the basic architecture of the pod software by reviewing [its simplified version](https://github.com/Hyp-ed/hyped-demo/tree/develop/src).
But talking and doing are very different things. By implementing something even simpler that still follows the same structure as the real pod software,
you should get an even more useful perspective on the pod software.

The task is to implement a controller for a simple imaginary oven described in this [video](https://youtu.be/-Yicg2TTMPs). It has two user inputs: an
on/off switch and a temperature dial.

I suggest the oven controller has 4 modules:
* **Thermometer module** - updates the current temperature inside the oven
* **Heating element controller module** - adds heat to the oven when state machine is in the appropriate state (otherwise the oven slowly loses heat,
  but don't worry about realistic physics, make it simple)
* **User interface module** - watches the on/off switch and the desired temperature dial in the following way:
  1. Wait for user input in terminal
  2. If it is an on/off command or a number representing the desired temperature, communicate the news to other modules
  3. Print some info about the current state of the oven (e.g. actual temperature and current state machine state)
  4. Repeat steps 1-4 indefinitely
* **State machine module** - implements the three states and the transitions from the [video](https://youtu.be/-Yicg2TTMPs)

Other uggestions:
* You can join the thermometer and heater into a single module to make things simpler.
* If you know of a better way to implement the user interface, please share. But this is not the point of the exercise, so you should spend as little
  time on it as possible.
* Copy stuff from the [pod code](https://github.com/Hyp-ed/hyped-demo/). You will probably want the Makefiles and some of the utils, especially `Thread`
  and maybe `Logger` and `System` (`System` processes command line arguments among other things).
* You can use group programming to do the initial setup (directory structure, Makefiles,...) - one person shares their screen and the rest of the group
  tells them what to do.
* Afterwards, you can split tasks and parallelise. You can use feature branches and pull requests to practice the actual HYPED workflow if you want.

### Rules
* Work in two groups (State Machine and QA) and create two copies of the oven controller. Create and use `stm-develop1` and `qa-develop1` branches
  in this repo for the implementations.
* Keep the architecture the same as the pod software has - central data structure for communication between modules, each module in its own thread
* Adhere to the HYPED [style guide](https://hyp-ed.github.io/styleguide/)
