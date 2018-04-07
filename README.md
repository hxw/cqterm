# cqterm

 QT based terminal on same lines as conlecterm

# functions

* left hand tabs with horizontal text
* tabs can be reordered by drag/drop
* run terminal (urxvt/xterm) in specified directory
* start/close buttons only when a terminal is inactive
* right click pop-up over tab bar to add new tab
* UCL configuration
    + commands like urxvt, xterm (emacs/vim should also work)
    + tabs to specify tab text, command, start directory and initial commands
* JSON session file keeps tab order
    + always save on exit
    + save on tab order change or tab append (*MISSING*)
* active tab indication
* use of `XDG_CONFIG_HOME` to store conf/session files


# to fix

* tab order changed => save-session
* click to focus on terminal
    +  mouse must be over termial window to receive input
