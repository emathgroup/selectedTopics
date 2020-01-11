import React from "react"
import Layout from '../components/layout'
import Helmet from 'react-helmet'
import '../styles/index.scss'
import config from '../../data/siteConfig'
import BlogPage from './blog'

const Indexpage = ()=>{
    return(
        <BlogPage/> 
        )
}

export default Indexpage
