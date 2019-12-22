""" Does a step-over then prints the local variables or only the ones passed in """
import lldb

class Wrap:
    def __init__(self, debugger, unused):
        return

    def __call__(self, debugger, command, exe_ctx, result):
        # Set the command to synchronous so the step will complete
        # before we try to run the frame variable.
        old_async = debugger.GetAsync()
        debugger.SetAsync(False)

        debugger.HandleCommand(command)

        print('LLDB DONE')
        debugger.SetAsync(old_async)

    def get_short_help(self):
        return "Does a command then prints LLDB DONE\n"

def __lldb_init_module(debugger, unused):
    debugger.HandleCommand("command script add -c wrapper.Wrap wrap")
