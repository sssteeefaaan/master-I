mod impl_mutex;
mod impl_channels;
mod impl_atomics;
mod impl_conditional_variables;

fn main() {
    impl_mutex::main();
}
