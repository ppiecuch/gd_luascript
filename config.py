
def can_build(env, platform):
    return env["tools"] or env["module_luascript_enabled"]


def configure(env):
    pass
