type t;
[@bs.module "duration-pretty"] external duration: int => t = "duration";
[@bs.send] external format: (t, string) => string = "format";