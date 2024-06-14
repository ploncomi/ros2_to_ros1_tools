alias rosnode='ros2 node'
alias rosservice='ros2 service'
alias rosbag='ros2 bag'

_roscd() 
{
    local cur prev opts

    COMPREPLY=()
    cur="${COMP_WORDS[COMP_CWORD]}"
    prev="${COMP_WORDS[COMP_CWORD-1]}"
    opts=$(roscd --list)

    if [[ ${cur} == * ]] ; then
        COMPREPLY=( $(compgen -W "${opts}" -- ${cur}) )
        return 0
    fi
}
complete -F _roscd roscd


function rostopic() {
    if [[ "info" = "$1" ]] ; then
      ros2 topic info "--verbose" "${@:2}"
    else
      ros2 topic "${@:1}"
    fi
}
