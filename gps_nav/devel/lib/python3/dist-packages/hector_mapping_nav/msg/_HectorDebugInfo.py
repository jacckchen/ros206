# This Python file uses the following encoding: utf-8
"""autogenerated by genpy from hector_mapping_nav/HectorDebugInfo.msg. Do not edit."""
import codecs
import sys
python3 = True if sys.hexversion > 0x03000000 else False
import genpy
import struct

import hector_mapping_nav.msg

class HectorDebugInfo(genpy.Message):
  _md5sum = "4d33c0696c0c536f5c1447c260756674"
  _type = "hector_mapping_nav/HectorDebugInfo"
  _has_header = False  # flag to mark the presence of a Header object
  _full_text = """HectorIterData[] iterData
================================================================================
MSG: hector_mapping_nav/HectorIterData
float64[9] hessian
float64 conditionNum
float64 determinant
float64 conditionNum2d
float64 determinant2d
"""
  __slots__ = ['iterData']
  _slot_types = ['hector_mapping_nav/HectorIterData[]']

  def __init__(self, *args, **kwds):
    """
    Constructor. Any message fields that are implicitly/explicitly
    set to None will be assigned a default value. The recommend
    use is keyword arguments as this is more robust to future message
    changes.  You cannot mix in-order arguments and keyword arguments.

    The available fields are:
       iterData

    :param args: complete set of field values, in .msg order
    :param kwds: use keyword arguments corresponding to message field names
    to set specific fields.
    """
    if args or kwds:
      super(HectorDebugInfo, self).__init__(*args, **kwds)
      # message fields cannot be None, assign default values for those that are
      if self.iterData is None:
        self.iterData = []
    else:
      self.iterData = []

  def _get_types(self):
    """
    internal API method
    """
    return self._slot_types

  def serialize(self, buff):
    """
    serialize message into buffer
    :param buff: buffer, ``StringIO``
    """
    try:
      length = len(self.iterData)
      buff.write(_struct_I.pack(length))
      for val1 in self.iterData:
        buff.write(_get_struct_9d().pack(*val1.hessian))
        _x = val1
        buff.write(_get_struct_4d().pack(_x.conditionNum, _x.determinant, _x.conditionNum2d, _x.determinant2d))
    except struct.error as se: self._check_types(struct.error("%s: '%s' when writing '%s'" % (type(se), str(se), str(locals().get('_x', self)))))
    except TypeError as te: self._check_types(ValueError("%s: '%s' when writing '%s'" % (type(te), str(te), str(locals().get('_x', self)))))

  def deserialize(self, str):
    """
    unpack serialized message in str into this message instance
    :param str: byte array of serialized message, ``str``
    """
    if python3:
      codecs.lookup_error("rosmsg").msg_type = self._type
    try:
      if self.iterData is None:
        self.iterData = None
      end = 0
      start = end
      end += 4
      (length,) = _struct_I.unpack(str[start:end])
      self.iterData = []
      for i in range(0, length):
        val1 = hector_mapping_nav.msg.HectorIterData()
        start = end
        end += 72
        val1.hessian = _get_struct_9d().unpack(str[start:end])
        _x = val1
        start = end
        end += 32
        (_x.conditionNum, _x.determinant, _x.conditionNum2d, _x.determinant2d,) = _get_struct_4d().unpack(str[start:end])
        self.iterData.append(val1)
      return self
    except struct.error as e:
      raise genpy.DeserializationError(e)  # most likely buffer underfill


  def serialize_numpy(self, buff, numpy):
    """
    serialize message with numpy array types into buffer
    :param buff: buffer, ``StringIO``
    :param numpy: numpy python module
    """
    try:
      length = len(self.iterData)
      buff.write(_struct_I.pack(length))
      for val1 in self.iterData:
        buff.write(val1.hessian.tostring())
        _x = val1
        buff.write(_get_struct_4d().pack(_x.conditionNum, _x.determinant, _x.conditionNum2d, _x.determinant2d))
    except struct.error as se: self._check_types(struct.error("%s: '%s' when writing '%s'" % (type(se), str(se), str(locals().get('_x', self)))))
    except TypeError as te: self._check_types(ValueError("%s: '%s' when writing '%s'" % (type(te), str(te), str(locals().get('_x', self)))))

  def deserialize_numpy(self, str, numpy):
    """
    unpack serialized message in str into this message instance using numpy for array types
    :param str: byte array of serialized message, ``str``
    :param numpy: numpy python module
    """
    if python3:
      codecs.lookup_error("rosmsg").msg_type = self._type
    try:
      if self.iterData is None:
        self.iterData = None
      end = 0
      start = end
      end += 4
      (length,) = _struct_I.unpack(str[start:end])
      self.iterData = []
      for i in range(0, length):
        val1 = hector_mapping_nav.msg.HectorIterData()
        start = end
        end += 72
        val1.hessian = numpy.frombuffer(str[start:end], dtype=numpy.float64, count=9)
        _x = val1
        start = end
        end += 32
        (_x.conditionNum, _x.determinant, _x.conditionNum2d, _x.determinant2d,) = _get_struct_4d().unpack(str[start:end])
        self.iterData.append(val1)
      return self
    except struct.error as e:
      raise genpy.DeserializationError(e)  # most likely buffer underfill

_struct_I = genpy.struct_I
def _get_struct_I():
    global _struct_I
    return _struct_I
_struct_4d = None
def _get_struct_4d():
    global _struct_4d
    if _struct_4d is None:
        _struct_4d = struct.Struct("<4d")
    return _struct_4d
_struct_9d = None
def _get_struct_9d():
    global _struct_9d
    if _struct_9d is None:
        _struct_9d = struct.Struct("<9d")
    return _struct_9d
