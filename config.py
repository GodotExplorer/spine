

def can_build(env, platform):
  return True
  
  
def configure(env):
  #env.Append(CPPFLAGS=['-DNEED_LONG_INT'])
  pass
  
  
  
def get_doc_classes():
  return [
    "Spine",
  ]