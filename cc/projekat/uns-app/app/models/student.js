const {getClient} = require("../utils/database")
const {errors} = require("./studentErrors")
const bcrypt = require('bcrypt')
const SALT_WORK_FACTOR = parseInt(process.env["SALT_WORK_FACTOR"] || '10')

const studentSchema = new getClient().Schema({
    firstName: {
        type: String,
        minLength: [1, errors.firstName.minLength],
        trim: true,
        required: [true, errors.firstName.required]
    },
    lastName: {
        type: String,
        minLength: [1, errors.lastName.minLength],
        trim: true,
        required: [true, errors.lastName.required]
    },
    index: {
        type: String,
        trim: true,
        required: [true, errors.index.required],
        unique: [true, errors.index.unique],
        index: true,
        validate: {
            validator: v => /[a-z0-9\-]+/.test(v),
            message: errors.index.invalid
        },
    },
    faculty: {
        type: String,
        required: [true, errors.faculty.required],
        uppercase: true,
        enum: {
            values: ["FTN", "PMF", "PRAVNI"],
            message: errors.faculty.enum
        }
    },
    email: {
        type: String,
        trim: true,
        lowercase: true,
        unique: [true, errors.email.unique],
        required: [true, errors.email.required],
        index: true,
        validate: {
            validator: v => /^\w+([\.-]?\w+)*@\w+([\.-]?\w+)*(\.\w{2,3})+$/.test(v),
            message: errors.email.invalid
        }
    },
    username: {
        type: String,
        trim: true,
        lowercase: true,
        unique: [true, errors.username.unique],
        required: [true, errors.username.required],
        index: true,
        validate: {
            validator: v => /^[A-Za-z]+([\._]?[A-Za-z0-9]+)*$/.test(v),
            message: errors.username.invalid
        }
    },
    password: {
        type: String,
        minLength: [8, errors.password.minLength],
        trim: true,
        required: [true, errors.password.required]
    },
    umcn: {
        type: String,
        trim: true,
        unique: [true, errors.umcn.unique],
        required: [true, errors.umcn.required],
        index: true,
        validate: {
            validator: v => /^[0-9]{13}$/.test(v),
            message: errors.umcn.invalid
        }
    },
    idCardNumber: {
        type: String,
        trim: true,
        unique: [true, errors.idCardNumber.unique],
        required: [true, errors.idCardNumber.required],
        index: true,
        validate: {
            validator: v => /^[0-9]{7,10}$/.test(v),
            message: errors.idCardNumber.invalid
        }
    },
    phoneNumber: {
        type: String,
        trim: true,
        unique: [true, errors.phoneNumber.unique],
        required: [true, errors.phoneNumber.required],
        index: true,
        validate: {
            validator: v => /^\+?[0-9]{9,13}$/.test(v),
            message: errors.phoneNumber.invalid
        }
    },
    image: {
        type: String
    },
    birthDate: {
        type: Date,
        required: [true, errors.birthDate.required]
    },
    address: {
        type: {
            postalCode: {
                type: Number,
                required: [true, errors.address.postalCode.required]
            },
            municipality: {
                type: String,
                trim: true,
                required: [true, errors.address.municipality.required]
            },
            name: {
                type: String,
                trim: true,
                required: [true, errors.address.name.required]
            }
        },
        required: [true, errors.address.required]
    },
    parent: {
        type: {
            firstName: {
                type: String,
                trim: true,
                required: [true, errors.parent.firstName.required]
            },
            lastName: {
                type: String,
                trim: true,
                required: [true, errors.parent.lastName.required]
            },
            umcn: {
                type: String,
                trim: true,
                required: [true, errors.parent.umcn.required],
                validate: {
                    validator: v => /^[0-9]{13}$/.test(v),
                    message: errors.parent.umcn.invalid
                }
            },
            idCardNumber: {
                type: String,
                trim: true,
                required: [true, errors.parent.idCardNumber.required],
                validate: {
                    validator: v =>/^[0-9]{8,10}$/.test(v),
                    message: errors.parent.idCardNumber.invalid
                }
            },
            phoneNumber: {
                type: String,
                trim: true,
                required: [true, errors.parent.phoneNumber.required],
                validate: {
                    validator: v => /^\+?[0-9]{9,14}$/.test(v),
                    message: errors.parent.phoneNumber.invalid
                }
            },
            address: {
                type:{
                    postalCode: {
                        type: Number,
                        trim: true,
                        required: [true, errors.parent.address.postalCode.required]
                    },
                    municipality: {
                        type: String,
                        trim: true,
                        required: [true, errors.parent.address.municipality.required]
                    },
                    name: {
                        type: String,
                        trim: true,
                        required: [true,errors.parent.address.name.required]
                    }
                },
                required: [true, errors.parent.address.required]
            }
        },
        required: [true, errors.parent.required]
    },
});

studentSchema.pre('save', function(next) {
    const user = this;

    // only hash the password if it has been modified (or is new)
    if (!user.isModified('password')) return next();

    // generate a salt
    bcrypt.genSalt(SALT_WORK_FACTOR, function(err, salt) {
        if (err) return next(err);

        // hash the password using our new salt
        bcrypt.hash(user.password, salt, function(err, hash) {
            if (err) return next(err);
            // override the cleartext password with the hashed one
            user.password = hash;
            next();
        });
    });
});

studentSchema.methods.comparePassword = async function(password) {
   return await bcrypt.compare(password, this.password)
}

const Student = getClient().model("Student", studentSchema, "students")
module.exports = { Student }