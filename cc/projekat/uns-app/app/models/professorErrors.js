const errors = {
    firstName: {
        minLength: "Professor's first name needs to be at least 1 character long!",
        required: "Professor's first name cannot be empty!",
    },
    lastName: {
        minLength: "Professor's last name needs to be at least 1 character long!",
        required: "Professor's last name cannot be empty!"
    },
    email:{
        unique: "Professor with that email address already exists!",
        required: "Email address cannot be empty!",
        invalid: "Please provide a valid email address!"
    },
    username:{
        unique: "Professor with that username already exists!",
        required: "Username cannot be empty!",
        invalid: "Please provide a valid username!"
    },
    faculty:{
        required: "Faculty cannot be empty!",
        enum: "Faculty can only be one of [FTN, PMF, PRAVNI]"
    },
    password:{
        minLength: "Password needs to be at least 8 characters long!",
        required: "Password cannot be empty!"
    },
    umcn: {
        unique: "Professor with that Unique Master Citizen Number (UMCN) already exists!",
        required: "Professor's Unique Master Citizen Number (UMCN) cannot be empty!",
        invalid: "Please provide a valid Unique Master Citizen Number for the professor!"
    },
    idCardNumber:{
        unique: "Professor with that Identification Card Number (ID) already exists!",
        required: "Professor's identification card number cannot be empty!",
        invalid: "Please provide a valid identification card number for the professor!",
    },
    phoneNumber:{
        unique: "Professor with that phone number already exists!",
        required: "Professor's phone number cannot be empty!",
        invalid: "Please provide a valid phone number for the professor!",
    },
    address:{
        required: "Professor's address information cannot be empty!",
        postalCode: {
            required: "Professor's postal code cannot be empty"
        },
        municipality:{
            required: "Professor's address municipality cannot be empty"
        },
        name:{
            required: "Professor's address name cannot be empty"
        }
    }
}

module.exports = {errors}