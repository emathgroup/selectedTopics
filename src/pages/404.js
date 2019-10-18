import React from 'react'
import { Link } from 'gatsby'
import Layout from '../components/layout'
import Helmet from 'react-helmet'
const NotFound =()=>{
    return(
        <Layout>
            <Helmet title={`Page Not Found`}/>
            <center><h1>Page Not Found</h1>
            <span><Link to="/">Go to Home</Link></span>
            </center>
            
        </Layout>
    )
}
export default NotFound