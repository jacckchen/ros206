#!/bin/sh

if [ -n "$DESTDIR" ] ; then
    case $DESTDIR in
        /*) # ok
            ;;
        *)
            /bin/echo "DESTDIR argument must be absolute... "
            /bin/echo "otherwise python's distutils will bork things."
            exit 1
    esac
fi

echo_and_run() { echo "+ $@" ; "$@" ; }

echo_and_run cd "/home/agrotech/fz/src/arbotix_ros/arbotix_python"

# ensure that Python install destination exists
echo_and_run mkdir -p "$DESTDIR/home/agrotech/fz/install/lib/python3/dist-packages"

# Note that PYTHONPATH is pulled from the environment to support installing
# into one location when some dependencies were installed in another
# location, #123.
echo_and_run /usr/bin/env \
    PYTHONPATH="/home/agrotech/fz/install/lib/python3/dist-packages:/home/agrotech/fz/build/lib/python3/dist-packages:$PYTHONPATH" \
    CATKIN_BINARY_DIR="/home/agrotech/fz/build" \
    "/usr/bin/python3" \
    "/home/agrotech/fz/src/arbotix_ros/arbotix_python/setup.py" \
    egg_info --egg-base /home/agrotech/fz/build/arbotix_ros/arbotix_python \
    build --build-base "/home/agrotech/fz/build/arbotix_ros/arbotix_python" \
    install \
    --root="${DESTDIR-/}" \
    --install-layout=deb --prefix="/home/agrotech/fz/install" --install-scripts="/home/agrotech/fz/install/bin"
