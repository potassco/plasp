# Incremental STRIPS Planning Encodings

This suite of incremental STRIPS planning encodings implements diverse methods.
The included encoding files provide the following functionalities:

## Encodings

### [incmode.lp](incmode.lp): include built-in “iclingo“ script for incremental grounding and solving

### [preprocess.lp](preprocess.lp): static analysis of potentially relevant actions

* Parameter: `_closure` (default value: `3`)
  * Value `1`: forward chaining of effects w.r.t. initial variable values
  * Value `2`: backward regression of effects w.r.t. goal variable values
  * Value `3`: both forward chaining and backward regression of effects
  * Otherwise: off (simply take all actions as given)

### [strips-incremental.lp](strips-incremental.lp): sequential and parallel planning encoding variants
* Parameter: `_parallel` (default value: `0`)
  * Value `1`: “forall” parallel actions that can be arranged in any sequence
  * Value `2`: “exists” parallel actions that can be arranged in some sequence
  * Value `3`: “exists“ parallel actions omitting achievement of preconditions
  * Value `4`: “exists“ parallel actions like `3` implemented by acyclicity
  * Otherwise: sequential actions
* Parameter: `planner_on` (default value: `0`)
  * Value `0`: no compatibility to ASP-based planner (no idle steps permitted)
  * Otherwise: compatibility to ASP-based planner (idle steps permitted)

### [redundancy.lp](redundancy.lp): enforcement of ‘redundant’ actions to constrain parallel plans
* Remarks:
  * Only relevant together with parallel actions not implemented by acyclicity
  * Encoded constraints seem rather ineffective though
  * Heavy space overhead in combination with “exists” parallel actions

### [postprocess.lp](postprocess.lp): plan feasibility checking and conversion to sequential plan

## Usage Examples

Some example invocations (using `clingo` 5.1.0) are as follows:

```bash
plasp ../../instances/PDDL/ipc-2000-elevator-m10-strips/domain.pddl ../../instances/PDDL/ipc-2000-elevator-m10-strips/problem-04-00.pddl | clingo - incmode.lp preprocess.lp strips-incremental.lp

plasp ../../instances/PDDL/ipc-2000-elevator-m10-strips/domain.pddl ../../instances/PDDL/ipc-2000-elevator-m10-strips/problem-04-00.pddl | clingo - incmode.lp preprocess.lp strips-incremental.lp -c _closure=0

plasp ../../instances/PDDL/ipc-2000-elevator-m10-strips/domain.pddl ../../instances/PDDL/ipc-2000-elevator-m10-strips/problem-04-00.pddl | clingo - incmode.lp preprocess.lp strips-incremental.lp -c _closure=1

plasp ../../instances/PDDL/ipc-2000-elevator-m10-strips/domain.pddl ../../instances/PDDL/ipc-2000-elevator-m10-strips/problem-04-00.pddl | clingo - incmode.lp preprocess.lp strips-incremental.lp -c _closure=2

plasp ../../instances/PDDL/ipc-2000-elevator-m10-strips/domain.pddl ../../instances/PDDL/ipc-2000-elevator-m10-strips/problem-04-00.pddl | clingo - incmode.lp preprocess.lp strips-incremental.lp -c _parallel=1

plasp ../../instances/PDDL/ipc-2000-elevator-m10-strips/domain.pddl ../../instances/PDDL/ipc-2000-elevator-m10-strips/problem-04-00.pddl | clingo - incmode.lp preprocess.lp strips-incremental.lp -c _parallel=2

plasp ../../instances/PDDL/ipc-2000-elevator-m10-strips/domain.pddl ../../instances/PDDL/ipc-2000-elevator-m10-strips/problem-04-00.pddl | clingo - incmode.lp preprocess.lp strips-incremental.lp -c _parallel=3

plasp ../../instances/PDDL/ipc-2000-elevator-m10-strips/domain.pddl ../../instances/PDDL/ipc-2000-elevator-m10-strips/problem-04-00.pddl | clingo - incmode.lp preprocess.lp strips-incremental.lp -c _parallel=4

plasp ../../instances/PDDL/ipc-2000-elevator-m10-strips/domain.pddl ../../instances/PDDL/ipc-2000-elevator-m10-strips/problem-04-00.pddl | clingo - incmode.lp preprocess.lp strips-incremental.lp -c _parallel=1 redundancy.lp

plasp ../../instances/PDDL/ipc-2000-elevator-m10-strips/domain.pddl ../../instances/PDDL/ipc-2000-elevator-m10-strips/problem-04-00.pddl | clingo - incmode.lp preprocess.lp strips-incremental.lp -c _parallel=2 redundancy.lp

plasp ../../instances/PDDL/ipc-2000-elevator-m10-strips/domain.pddl ../../instances/PDDL/ipc-2000-elevator-m10-strips/problem-04-00.pddl | clingo - incmode.lp preprocess.lp strips-incremental.lp -c _parallel=3 redundancy.lp

plasp ../../instances/PDDL/ipc-2000-elevator-m10-strips/domain.pddl ../../instances/PDDL/ipc-2000-elevator-m10-strips/problem-04-00.pddl | clingo - incmode.lp preprocess.lp strips-incremental.lp --outf=1 | grep -A1 -e "ANSWER" | tail -n1 | clingo - postprocess.lp <(plasp ../../instances/PDDL/ipc-2000-elevator-m10-strips/domain.pddl ../../instances/PDDL/ipc-2000-elevator-m10-strips/problem-04-00.pddl)

plasp ../../instances/PDDL/ipc-2000-elevator-m10-strips/domain.pddl ../../instances/PDDL/ipc-2000-elevator-m10-strips/problem-04-00.pddl | clingo - incmode.lp preprocess.lp strips-incremental.lp --outf=1 -c _parallel=1 | grep -A1 -e "ANSWER" | tail -n1 | clingo - postprocess.lp <(plasp ../../instances/PDDL/ipc-2000-elevator-m10-strips/domain.pddl ../../instances/PDDL/ipc-2000-elevator-m10-strips/problem-04-00.pddl)

plasp ../../instances/PDDL/ipc-2000-elevator-m10-strips/domain.pddl ../../instances/PDDL/ipc-2000-elevator-m10-strips/problem-04-00.pddl | clingo - incmode.lp preprocess.lp strips-incremental.lp --outf=1 -c _parallel=2 | grep -A1 -e "ANSWER" | tail -n1 | clingo - postprocess.lp <(plasp ../../instances/PDDL/ipc-2000-elevator-m10-strips/domain.pddl ../../instances/PDDL/ipc-2000-elevator-m10-strips/problem-04-00.pddl)

plasp ../../instances/PDDL/ipc-2000-elevator-m10-strips/domain.pddl ../../instances/PDDL/ipc-2000-elevator-m10-strips/problem-04-00.pddl | clingo - incmode.lp preprocess.lp strips-incremental.lp --outf=1 -c _parallel=3 | grep -A1 -e "ANSWER" | tail -n1 | clingo - postprocess.lp <(plasp ../../instances/PDDL/ipc-2000-elevator-m10-strips/domain.pddl ../../instances/PDDL/ipc-2000-elevator-m10-strips/problem-04-00.pddl)

plasp ../../instances/PDDL/ipc-2000-elevator-m10-strips/domain.pddl ../../instances/PDDL/ipc-2000-elevator-m10-strips/problem-04-00.pddl | clingo - incmode.lp preprocess.lp strips-incremental.lp --outf=1 -c _parallel=4 | grep -A1 -e "ANSWER" | tail -n1 | clingo - postprocess.lp <(plasp ../../instances/PDDL/ipc-2000-elevator-m10-strips/domain.pddl ../../instances/PDDL/ipc-2000-elevator-m10-strips/problem-04-00.pddl)
```
