import React from "react"
import Layout from '../components/layout'
import Helmet from 'react-helmet'
import '../styles/index.scss'
import config from '../../data/siteConfig'
const Indexpage = ()=>{
    return(
    <Layout>
        <Helmet title={config.siteTitle}/>
        <div>
        <h1>数学研发论坛</h1>
        <p>本<a href="/blog">网站</a>主要是搜集<a href="https://bbs.emath.ac.cn/forum.php">数学研发论坛</a>上的精华帖子. </p>
        
        </div>
    </Layout>
        )
}

export default Indexpage
