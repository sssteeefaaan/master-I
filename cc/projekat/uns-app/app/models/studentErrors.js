const errors = {
    firstName: {
        minLength: "Student's first name needs to be at least 1 character long!",
        required: "Student's first name cannot be empty!",
    },
    lastName: {
        minLength: "Student's last name needs to be at least 1 character long!",
        required: "Student's last name cannot be empty!"
    },
    index:{
        required: "Index number cannot be empty!",
        unique:"Student with that index number already exists!",
        invalid: "Please provide a valid index number!"
    },
    faculty:{
        required: "Faculty cannot be empty!",
        enum: "Faculty can only be one of [FTN, PMF, PRAVNI]"
    },
    email:{
        unique: "Student with that email address already exists!",
        required: "Email address cannot be empty!",
        invalid: "Please provide a valid email address!"
    },
    username:{
        unique: "Student with that username already exists!",
        required: "Username cannot be empty!",
        invalid: "Please provide a valid username!"
    },
    password:{
        minLength: "Password needs to be at least 8 characters long!",
        required: "Password cannot be empty!"
    },
    umcn: {
        unique: "Student with that Unique Master Citizen Number (UMCN) already exists!",
        required: "Student's Unique Master Citizen Number (UMCN) cannot be empty!",
        invalid: "Please provide a valid Unique Master Citizen Number for the student!"
    },
    idCardNumber:{
        unique: "Student with that Identification Card Number (ID) already exists!",
        required: "Student's identification card number cannot be empty!",
        invalid: "Please provide a valid identification card number for the student!",
    },
    phoneNumber:{
        unique: "Student with that phone number already exists!",
        required: "Student's phone number cannot be empty!",
        invalid: "Please provide a valid phone number for the student!",
    },
    birthDate:{
        required: "Student's birth date cannot be empty!"
    },
    address:{
        required: "Student's address information cannot be empty!",
        postalCode: {
            required: "Student's postal code cannot be empty"
        },
        municipality:{
            required: "Student's address municipality cannot be empty"
        },
        name:{
            required: "Student's address name cannot be empty"
        }
    },
    parent:{
        required: "Student's parent information cannot be empty!",
        firstName: {
            required: "Parent's first name cannot be empty!",
        },
        lastName:{
            required: "Parent's last name cannot be empty!"
        },
        umcn:{
            required: "Parent's UMCN cannot be empty!",
            invalid: "Please provide a valid unique master citizen number for the student's parent!"
        },
        idCardNumber:{
            required: "Parent's ID cannot be empty!",
            invalid: "Please provide a valid identification card number for the student's parent!"
        },
        phoneNumber:{
            required:"Parent's phone number cannot be empty!",
            invalid: "Please provide a valid phone number for the student's parent!"
        },
        address:{
            required: "Parent's address cannot be empty!",
            postalCode:{
                required: "Parent's address postal code cannot be empty!"
            },
            municipality:{
                required: "Parent's address municipality cannot be empty!"
            },
            name:{
                required:  "Parent's address name cannot be empty!"
            }
        }
    }
}

module.exports = {errors}