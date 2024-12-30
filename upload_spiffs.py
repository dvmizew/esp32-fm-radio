Import("env")

def before_upload(source, target, env):
    # run the SPIFFS upload automatically before the main upload
    env.Execute("platformio run --target uploadfs")

env.AddPreAction("upload", before_upload)