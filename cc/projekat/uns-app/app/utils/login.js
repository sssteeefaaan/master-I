const {Student} = require("../models/student")

async function loginStudent(data){
    const filter = []
    for(k of Object.keys(data))
        if(k != "password")
            filter.push({ [k]: data[k] })
    console.log(filter)
    if(filter.length == 0)
        return { status: 400, message: "Invalid credentials provided!", content: null }
    const res = await Student.findOne({
        $or: filter
    })
    console.log(res)
    if(res != null){
        const match = await res.comparePassword(data["password"])
        if(match) return { status: 200, message: "Success!", content: res }
        else return { status: 400, message: "Wrong password!", content: null }
    }
    return {status: 400, message: "Student not found!", content: null }
}

async function loginProfessor(data){

}

module.exports = { loginStudent, loginProfessor }