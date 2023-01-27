const {Student} = require("../models/student")
const {Professor} = require("../models/professor")
const errorList = {
    student: require("../models/studentErrors").errors,
    professor: require("../models/professorErrors").errors
}

function parseErrors(error, type){
    // Unique constraint is validated on the mongo server side, so mongoose cannot read that error (11000)
    let errors = []
    if (error.code == "11000"){
        for(let field of Object.keys(error.keyValue)){
            const key = field.split(".")
            let inner = errorList[type]
            for (k of key){
                if(inner == null)
                    break
                inner = inner[k]
            }
            errors.push(inner.unique || `${field} must be unique!`)
        }
    }else{
        let errors = []
        for(let er of Object.keys(error.errors))
            errors.push(error.errors[er].properties.message)
        return errors
    }
    return errors
}


async function registerStudent(data){
    console.log("Register student", data)
    if(!data.sservice)
        return [`Empty student service '${data.sservice}'!`]
    if(!data.student)
        return ["Empty student data!"]
    try{
        await Student.create({ ...data.student, faculty: data.sservice })
        return []
    }catch(e){
        const parsedErrors = parseErrors(e, "student")
        console.log(parsedErrors)
        return parsedErrors
    }
}

async function registerProfessor(data){
    console.log("Register professor", data)
    if(!data.sservice)
        return [`Empty student service '${data.sservice}'!`]
    if(!data.professor)
        return ["Empty professor data!"]
    try{
        await Professor.create({ ...data.professor, faculty: data.sservice })
        return []
    }catch(e){
        const parsedErrors = parseErrors(e, "professor")
        console.log(parsedErrors)
        return parsedErrors
    }
}

module.exports = {
    registerStudent,
    registerProfessor
}