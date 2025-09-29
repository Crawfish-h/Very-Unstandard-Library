from enum import StrEnum
from datetime import datetime

class Route_Method(StrEnum):
    Get = "GET"
    Post = "POST"

def Empty_Func(cause):
    print("Empty_Func")
    pass

class Cause:
    def __init__(self, message, func):
        self.Message = message
        self.Func = func

class Ok(Cause):
    def __init__(self, message, func = Empty_Func):
        super().__init__(message, func)


class Fail(Cause):
    def __init__(self, message, func = Empty_Func):
        super().__init__(message, func)

class Result:
    def __init__(self, cause_List = []):
            self.Causes = cause_List
            self.Oks_Num = 0
            self.Fails_Num = 0

            for c in self.Causes:
                if isinstance(c, Ok):
                    self.Oks_Num += 1
                else:
                    self.Fails_Num += 1

    def Add(self, cause):
        self.Causes.append(cause)

        for c in self.Causes:
            if isinstance(c, Ok):
                self.Oks_Num += 1
            else:
                self.Fails_Num += 1

    def Run(self, *args):
        for c in self.Causes:
            if c.Func != Empty_Func:
                c.Func(c, *args)

def Msg_Print(cause):
    time = datetime.datetime.now()
    format_Time = time
    print("MESSAGE: - - - - - " + datetime.datetime.now() + cause.Message)

result = Result(
    [
        Ok("Function operation was sucessful"),
        Ok("Addition had no errors."),
        Fail("Operation failed", lambda f: 1 + 2)
    ]
)

result.Add(Fail("Failure"))

for c in result.Causes:
    print(c.Message)


result.Run()