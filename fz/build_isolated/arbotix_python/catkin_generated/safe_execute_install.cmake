execute_process(COMMAND "/home/agrotech/fz/build_isolated/arbotix_python/catkin_generated/python_distutils_install.sh" RESULT_VARIABLE res)

if(NOT res EQUAL 0)
  message(FATAL_ERROR "execute_process(/home/agrotech/fz/build_isolated/arbotix_python/catkin_generated/python_distutils_install.sh) returned error code ")
endif()
