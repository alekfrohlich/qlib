# Git-style

New features should be placed on branches of the same name. I.g: the development
of a keyboard device driver should happen in a branch named `keyboard`. In order
to commit such implementation into the master branch, all comits must be squashed
into one big PR commit with a descriptive commit message.

When a polemic design decision must be made, it should first be discussed on a
new open issue, with each side presenting rational reasons to why it's proposed
solution is better in the long run.