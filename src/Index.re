/* file: Index.re */
switch (ReactDOM.querySelector("#root")) {
| Some(root) => ReactDOM.render(<Timer />, root)
| None => ()
}