import React, { Component } from 'react';
import './App.css';
import { Table, Card } from 'semantic-ui-react'
import Button from './button'

class App extends Component {
  constructor(props){
    super(props)

    this.state = {
      recentData : [],
      allData : [],
      toggle : false
    }
  }


  componentDidMount(){
    this.fetchRecentData();
    setInterval(this.fetchRecentData, 5000);
}

fetchallData = () => {
  fetch('/data')
  .then(res => res.json())
  .then(allData => {
      this.setState({allData});
      console.log(allData);
  }); 

}

fetchRecentData = () => {
  fetch('/latest')
  .then(res => res.json())
  .then(recentData => {
      this.setState({recentData});
      console.log(recentData);
  }); 

}

toggleData = () =>{
  this.setState({toggle: !this.state.toggle});
  if(!this.state.toggle){
    this.fetchallData();
   setInterval(this.fetchallData, 10000);
  }
}

  render() {

    const history = this.state.allData.map((data) => {
      return (
        <Table.Row textAlign='center' key={data.gc_pub_sub_id}> 
            <Table.Cell className={data.data === 'Access granted' ? 'Granted' : 'Denied'}>{data.data}</Table.Cell>
            <Table.Cell>{data.published_at}</Table.Cell>
            <Table.Cell>{data.timestamp}</Table.Cell>
        </Table.Row>
    );
});

const recent = this.state.recentData.map((data) =>{
return (
  <Card className="Card" key={data.gc_pub_sub_id}>
  <Card.Content header='Recent Activity : Last Checked'></Card.Content>
  <Card.Content className={data.data === 'Access granted' ? 'Granted' : 'Denied'}>{data.data}</Card.Content>
  <Card.Content description={data.published_at}>
  </Card.Content>
</Card>
)

})

    return (
    <div className="App">
        <div className="ui centered card container">
    {recent}
    </div>
    <div className="ui container">
      <h1>History</h1>
      <Button onClick={this.toggleData} toggle={this.toggleData}></Button>
        {this.state.toggle ?
           <Table celled fixed>  
            <Table.Header>
                <Table.Row textAlign='center'>
                    <Table.HeaderCell>What</Table.HeaderCell>
                    <Table.HeaderCell>Date</Table.HeaderCell>
                    <Table.HeaderCell>Timestamp</Table.HeaderCell>
                </Table.Row>
            </Table.Header>
            <Table.Body>
                {history}
            </Table.Body>
        </Table> : ''}
    </div>
    </div>
    );
  }
}

export default App;
